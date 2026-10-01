#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/timer/sccp1.h"
#include "mcc_generated_files/pwm/sccp2.h"
#include "mcc_generated_files/adc/adc1.h"
#include "mcc_generated_files/qei/qei1.h"
#include "mcc_generated_files/uart/uart1.h"

#include "lookup_table.h"

#include "stdbool.h"
#include "stdio.h"
#include "math.h"

/*
 *Glavni upravljacki program jednoosnog Cubli sustava.
 * 
 *Program obuhvaca:
 * - ocitanje polozaja tijela pomocu inkrementalnog enkodera,
 * - izracun kutne brzine tijela,
 * - ocitanje stvarne struje motora i brzine zamasnjaka preko ADC-a
 * - izvodenje Swing-Up, LQR i Swing-Down upravljackih algoritama,
 * - promjenu nacina rada sustava,
 * - ogranicavanje struje s obzirom na napon i strujno ogranicenje,
 * - generiranje PWM reference za pogonski pretvarac ESCON,
 * - sigurnosni nadzor pogona i
 * - slanje mjernih velicina prema racunalu putem UART komunikacije.
 * 
 * Upravljacki i mjerni ciklus izvodi se svakih 5 ms.
*/

/* Nacin rada upravljackog sustava.
 * 
 * CTRL_SWING - aktivno povecavanje energije sustava
 * CTRL_NULA  - slobodno gibanje uz nultu zadanu struju
 * CTRL_LQR   - stabilizacija tijela u gornjem ravnoteznom polozaju
 * CTRL_DOWN  - kontrolirano spustanje sustava
 */
typedef enum 
{
    CTRL_SWING = 0,
    CTRL_NULA,
    CTRL_LQR,
    CTRL_DOWN
} ControlMode;

/* Osnovne konstante mjerenja polozaja i brzine tijela. */
#define PI 3.1415926f
#define Ts 0.005f                   // Razdoblje upravljackog ciklusa [s]
#define CPR 500                     // Broj impulsa enkodera po okretaju
#define THETA_K (2.0f*PI/(4.0f*CPR))// Pretvorba kvadraturnih impulsa u radijane
#define W_K (THETA_K/Ts)            // Pretvorba promjene impulsa u rad/s

/* 
 * Ogranicenje pogona
 * 
 * RPM_MAX odgovara konfiguriranoj najvecoj brzini pogona,
 * I_MAX programskom strujnom ogranicenju, a U_MAX raspolozivom
 * istosmjernom naponu napajanja. 
 */
#define RPM_MAX 3000.0f
#define I_MAX 2.33f
static const float U_max = 24.0f;

/*
 * Parametri PWM signala kojim se ESCON-u zadaje referentna struja.
 * Faktor ispune ogranicen je na raspon od 15 % do 85 %, dok
 * 50 % predstavlja nultu referentnu struju.
 */
#define PWM_PERIOD 4000U
#define DUTY_MIN 15U
#define DUTY_MAX 85U

/* Fizikalni parametri sustava.
 * Vrijednosti se koriste u izracunu energije i ogranicavanju motora.
 */
static const float J_b   = 1.87e-3f;
static const float J_w   = 5.43e-5f;
static const float l     = 84.85e-3f;
static const float l_b   = 69.365e-3f;
static const float m_b   = 0.2481f;
static const float m_w   = 0.052f;

static const float R     = 1.03f;
static const float Kt    = 33.5e-3f;
static const float g     = 9.81f;

/* Ekvivalentni moment tromosti tijela i energija gornjeg polozaja */
#define J_ (J_b + m_w*l*l)
#define E0 ((m_w*l + m_b*l_b)*g*2.0f)

/* Kutne granice za prijelaz izmedu upravljackih nacina.
 * Kut theta_norm jednak je nuli u gornjem ravnoteznom polozaju.
 */
static const float theta_up_granica = 0.2f;
static const float theta_down_granica = 0.3f;

/* 
 * Mrtva zona kutne brzine za pouzdano odredivanje smjera gibanja
 * i izbjegavanje promjene predznaka zbog mjernog suma.
 */
#define EPS_OMEGA 0.4f

/* Koeficijent sporog filtra za procjenu pomaka ravnoteznog kuta. */
static const float alpha_f = 0.002f;

/*
 * Parametri diskretne mreze tablice pretrazivanja dE.
 * Tablica je indeksirana preostalim kutom i apsolutnom kutnom brzinom tijela
 */
#define THETA_MIN 0.0f
#define OMEGA_MIN 1.0f
#define GRID_STEP 0.1f

/* 
 * Zastavice koje postavljaju prekidne rutine.
 * Oznaka volatile sprjecava optimizator da zanemari promjene nastale
 * izvan glavnog programskog toka.
 */
volatile bool control_tick = false;

volatile bool button_flag = false;
volatile bool clear_flag = false;
volatile bool down_flag = false;
volatile bool index_flag = false;

volatile bool speed_ready = false;
volatile bool current_ready = false;
volatile bool error_flag = false;

/* Sirove ADC vrijednosti analognih izlaza ESCON-a. */
volatile uint16_t adc_speed = 0;
volatile uint16_t adc_current = 0;

/* Korisnicke naredbe za ukljucivanje sustava i kontrolirano spustanje. */
bool system_enable = false;
bool down_request = false;

/* Sigurnosne i upravljacke zastavice glavnog programa. */
bool start_flag = false;
bool stop_flag = false;
bool last_swing = false;

/* Pocetni nacin rada nakon ukljucivanja sustava. */
static ControlMode mode = CTRL_SWING;

/* Varijable kvadraturnog enkodera tijela. */
int32_t enc_count = 0;          // Trenutacno stanje brojaca
int32_t enc_count_prev = 0;     // Stanje brojaca iz prethodnog ciklusa
int32_t delta = 0;              // Promjena broja impulsa tijekom 5 ms
bool skip_delta = false;        // Preskakanje brzine nakon resetiranja indeksom

/* Polozaj i kutna brzina tijela. */
float theta_enc = 0.0f;         // Enkoderski kut, donji polozaj odgovara nuli
float theta_norm = 0.0f;        // Kut normaliziran oko gornjeg polozaja
float omega_b = 0;              // Kutna brzina tijela [rad/s]

/* Izmjerene velicine pogona. */
float omega_w = 0.0f;           // Kutna brzina zamasnjaka [rad/s]
float I_act = 0.0f;             // Stvarna struja motora [A]

/* Uzorci medijanskog filtra brzine zamasnjaka i stvarne struje. */
static float w0 = 0.0f;
static float w1 = 0.0f;
static float w2 = 0.0f;
static float I0 = 0.0f;
static float I1 = 0.0f;
static float I2 = 0.0f;

/* Velicine povezane s procjenom energije tijekom Swing-Upa. */
float dtheta = 0;               // Preostali kut u trenutacnom smjeru gibanja
float dE = 0.0f;                // Razlika energije dohvacena iz tablice

float Ep    = 0.0f;             // Potencijalna energija
float Ek    = 0.0f;             // Ukupna kineticka energija
float Ekb    = 0.0f;            // Kineticka energija tijela
float E     = 0.0f;             // Ukupna mehanicka energija

/* Varijable za detekciju promjene smjera gibanja tijela. */
static int8_t omega_sign_prev = 0;
static bool omega_promjena_smjera = false;

/* Stanje filtra pomaka i konacna strujna referenca. */
float xf = 0.0f;
float I_out = 0.0f;

/* Izlazi pojedinih upravljackih algoritama. */
float I_swing = 0.0f;
float I_lqr = 0.0f;
float I_down = 0.0f;

/* Pomocna vrijednost brzine zamasnjaka u okretajima po minuti. */
float rpm = 0.0f;

/* Pocetnih 50 % faktora ispune predstavlja nultu strujnu referencu. */
uint16_t duty_cycle = PWM_PERIOD/2;

/* PREKIDNE RUTINE TIPKI I STATUSNIH SIGNALA */

/*Prekidna rutina tipke za ukljucivanje ili iskljucivanje sustava. */
void BUTTON(void)
{
    button_flag = true;
}

/*Prekidna rutina tipke za brisanje sigurnosnih zastavica. */
void B_CLEAR(void)
{
    clear_flag = true;
}

/* Prekidna rutina tipke za ukljucivanje ili iskljucivanje spustanja. */
void B_DOWN(void)
{
    down_flag = true;
}

/* 
 * Prekidna rutina ulaznog signala na prikljucku RE12.
 * Ako tijekom omogucenog rada ulaz poprimi stanje koje kod prepoznaje
 * kao gubitak spremnosti pogona, postavlja se sigurnosna zastavica pogreske.
 */
void ERROR(void)
{
    if(system_enable && !stop_flag && IO_RE12_GetValue())
    {
        error_flag = true;        
    }
}

/* POMOCNE FUNKCIJE I PROCJENA STANJA */

/*
 * Spori filtar prvog reda procjenjuje stalni pomak ravnoteznog kuta.
 * Procijenjena vrijednost xf koristi se kao korekcija u LQR regulatoru.
 */
void OffsetFilterUpdate(float theta_norm)
{
    xf += alpha_f*(theta_norm-xf);
}

/*
 * Odreduje predznak ulazne velicine uz mrtvu zonu eps.
 * Ako je apsolutna vrijednost manja od eps, zadrzava se prethodno
 * pouzdano odredeni predznak kako bi se smanjio utjecaj mjernog suma.
 */
static inline int8_t Direction(float x, float eps)
{
    static int8_t sign = 0;
    
    if(x > eps) sign = 1;
    if(x < -eps) sign = -1;
    return sign;
}

/*  
 * Provjera je li tijelo promijenilo smjer gibanja.
 * Promjena smjera koristi se pri odlucivanju o povratku iz nacina
 * nulte struje u Swing-Up nacin.
 */
void UpdateOmegaDirection(float omega_b)
{
    int8_t predznak = Direction(omega_b, EPS_OMEGA);
    
    if(predznak != 0 && omega_sign_prev != 0 && predznak != omega_sign_prev)
    {
        omega_promjena_smjera = true;
    }
    
    if(predznak != 0)
    {
        omega_sign_prev = predznak;
    }
}

/*
 * Izracunava energijske velicine sustava i vrijednost dE.
 * 
 * Parametar dtheta odreduje kutni put do gornjeg polozaja u trenutacnom
 * smjeru gibanja. Vrijednost dE dohvaca se iz unaprijed izracunate
 * tablice na temelju dtheta i apsolutne kutne brzine tijela.
 */
void Energija (float theta_enc, float theta_norm, float omega_b)
{
    /* Preslikavanje kuta u interval od 0 do 2*pi. */
    float theta_0_2pi = fmodf(theta_enc + 2.0f*PI, 2*PI);
    
    /* Odredivanje preostalog kuta prema trenutacnom smjeru gibanja. */
    if(omega_b <= 0.0f)
    {
        dtheta = 2.0f * PI - theta_0_2pi;
    }
    else
    {
        dtheta = theta_0_2pi;
    }
    
    /* Izracun potencijalne, kineticke i ukupne mehanicke energije. */
    Ep = (m_w*l + m_b*l_b)*g*(1.0f + cosf(theta_norm));
    Ek = 0.5f* J_ *omega_b*omega_b + 0.5f* J_w* (omega_b + omega_w)*(omega_b + omega_w);
    E = Ek + Ep;
    Ekb = 0.5* J_ *omega_b*omega_b;
    
    /* Za indeksiranje tablice koristi se apsolutna kutna brzina. */
    float omega_abs = fabsf(omega_b);
    
    /* Pretvorba kontinuiranih velicina u indekse tablice. */
    int row = (int)((dtheta - THETA_MIN)/GRID_STEP);
    int col = (int) ((omega_abs - OMEGA_MIN)/GRID_STEP);
    
    /* Ogranicavanje indeksa na raspolozive dimenzije tablice. */
    if(row < 0) row = 0;
    if (row >= ROWS) row = ROWS - 1;
    
    if(col < 0) col = 0;
    if(col >= COLS) col = COLS - 1;
    
    /* 
     * Vrijednosti u tablici pohranjene su uvecane 1000 puta,
     * pa se pri citanju ponovno pretvaraju u joule.
     */
    dE = dE_lookup[row][col]/1000.0f;
    
    /* Nenegativni dE oznacava zavrsetak aktivnog Swing-Up zamaha. */
    if(dE >= 0.0f) last_swing = true; //0.03
}

/* Normalizira kut u interval od -pi do pi. */
static inline float NormalizeAngle(float theta)
{
    theta = fmodf(theta + PI, 2.0f*PI);
    if(theta < 0.0f)
    {
        theta += 2.0f*PI;
    }
    return theta - PI;
}

/* Ogranicava vrijednost x na zadani interval. */
static inline float Clamp(float x, float min, float max)
{
    if(x > max) return max;
    if(x < min) return min;
    return x;
}

/*
 * Ogranicava strujnu referencu prema raspolozivom naponu i najvecoj
 * dopustenoj struji pogona.
 * 
 * Za zadanu struju najprije se procjenjuje potreban napon motora.
 * Nakon ogranicenja napona ponovno se racuna struja koju je moguce
 * ostvariti pri trenutacnoj brzini zamasnjaka.
 */
static inline float MotorLimit(float I_cmd, float omega_w)
{
    float U;
    float I;
    
    /* Napon potreban za ostvarivanje zadane struje. */
    U = I_cmd*R + Kt*omega_w;
    
    /* Ogranicavanje napona na raspolozivi napon napajanja. */
    U = Clamp(U, -U_max, U_max);
    
    /* Izracun ostvarive struje nakon naponskog ogranicenja. */
    I = (U - Kt*omega_w)/R;
    
    /* Zavrsno ogranicenje struje. */
    I = Clamp(I, -I_MAX, I_MAX);
    return I;
}

/* PROMJENA NACINA RADA I UPRAVLJACKI ALGORITMI */

/*
 * Upravlja prijelazima izmedu svih nacina rada sustava.
 * Odluka se donosi na temelju korisnickog zahtjeva, polozaja tijela,
 * parametra dE i promjene smjera gibanja.
 */
void Switch_Update(float theta_norm)
{
    float theta_abs = fabsf(theta_norm);
    switch(mode)
    {
        case CTRL_SWING:
            /* Zahtjev za spustanje ima prednost pred Swing-Up postupkom. */
            if(down_request)
            {
                mode = CTRL_DOWN;
            }
            /* U blizini gornjeg polozaja upravljanje preuzima LQR. */
            else if(theta_abs < theta_up_granica)
            {
                mode = CTRL_LQR;
            }
            /* Nakon posljednjeg aktivnog zamaha prelazi se na nultu struju. */
            else if(last_swing)
            {
                last_swing = false;
                mode = CTRL_NULA;
                omega_sign_prev = Direction(omega_b, EPS_OMEGA);
                omega_promjena_smjera = false;
            }
            break;
        
        case CTRL_NULA:
            /* Tijekom slobodnog gibanja prati se moguca promjena smjera. */
            UpdateOmegaDirection(omega_b);
            if(down_request)
            {
                mode = CTRL_DOWN;
            }
            else if (dE < 0)
            {
                
                /* Nedovoljna energija uzrokuje povratak na aktivni Swing-Up. */
                mode = CTRL_SWING;
            }
            /* Ulaskom u podrucje gornjeg polozaja aktivira se LQR. */
            else if(theta_abs < theta_up_granica)
            {
                mode = CTRL_LQR;
            }
            else if(omega_promjena_smjera)
            {
                /* Nakon promjene smjera ponovno se dodaje energija sustavu. */
                omega_promjena_smjera = false;
                mode = CTRL_SWING;
            }
            break;
            
        case CTRL_LQR:
            /* Korisnicka naredba pokrece kontrolirano spustanje. */
            if(down_request)
            {
                mode = CTRL_DOWN;
                xf = 0.0f;
            }
            else if(theta_abs > theta_down_granica)
            {
                /*
                 * Preveliko odstupanje od gornjeg polozaja smatra se
                 * gubitkom stabilizacije i pokrece kontrolirano spustanje.
                 */
                down_request = true;
                mode = CTRL_DOWN;
                xf = 0.0f;
            }
            break;
            
        case CTRL_DOWN:
            /* Iskljucivanje zahtjeva vraca sustav u pocetni Swing-Up nacin. */
            if(!down_request)
            {
                mode = CTRL_SWING;
            }
            break;
                    
        default:
            mode = CTRL_SWING;
            break;
    }
}

/* 
 * Swing-Up algoritam.
 * Smjer struje mijenja se s obzirom na smjer gibanja tijela, dok
 * amplituda struje iznosi polovinu programskog strujnog ogranicenja.
 */
void Swing_Update(float omega_b)
{
    static float I_step = I_MAX;

    if(omega_b > EPS_OMEGA)
    {
        I_step = -I_MAX;
    }
    else if(omega_b < -EPS_OMEGA)
    {
        I_step = I_MAX;
    }
    I_swing = I_step/2.0f;
}

/* 
 * LQR regulator koristi normalizirani kut tijela, kutnu brzinu tijela
 * i kutnu brzinu zamasnjaka. Vrijednost xf korigira ravnotezni kut.
 */
void LQR_Update(float theta_norm, float omega_b, float omega_w)
{
    static const float K_theta = -22.0f;
    static const float K_omega_b = -1.2775f;
    static const float K_omega_w = -0.0116f;
    
    I_lqr = - K_theta*(theta_norm-xf) - K_omega_b*omega_b - K_omega_w*omega_w;
}

/* 
 * Swing-Down algoritam prigusuje gibanje tijela i zamasnjaka.
 * Kada ukupna mehanicka energija padne ispod 0,01 J, zadana struja
 * postavlja se na nulu.
 */
void Swing_Down(float E, float omega_b, float omega_w)
{
    static const float K_omega_b = 1.5f;
    static const float K_omega_w = 0.05f;
    
    if(E<=0.01f)
    {
        I_down = 0.0f;
    }
    else
    {
        I_down = (K_omega_b*omega_b - K_omega_w*omega_w)*I_MAX;
    }
}

/* PREKIDNE I POMOCNE RUTINE PERIFERNIH MODULA */

/* Timer svakih 5 ms postavlja zastavicu novog upravljackog ciklusa. */
void TMR_5ms(void)
{
    control_tick = true;
}

/* Prekidna rutina indeksnog impulsa enkodera. */
void INDEX(void)
{
    index_flag = true;
}

/* Sprema zavrsnu ADC pretvorbu odgovarajuceg analognog kanala. */
void ADC_Interrupt(enum ADC_CHANNEL channel, uint16_t adcVal)
{
    if(channel == ACT_Speed)
    {
        adc_speed = adcVal;
        speed_ready = true;
    }
    else if(channel == ACT_Current)
    {
        adc_current = adcVal;
        current_ready = true;
    }
}

/*
 * Vraca medijan triju uzoraka i time uklanja pojedinacne impulsne
 * smetnje iz izmjerene brzine zamasnjaka i stvarne struje.
 */
static inline float median(float a, float b, float c)
{
    if(a > b){float t = a; a = b; b = t;}
    if(b > c){float t = b; b = c; c = t;}
    if(a > b){float t = a; a = b; b = t;}
    return b;
}

/* GLAVNI PROGRAM */
int main(void)
{
    /* Inicijalizacija sustava i perifernih modula generiranih pomocu MCC-a. */
    SYSTEM_Initialize();
    /* PWM modul SCCP2 generira referentni signal za ESCON. */
    SCCP2_PWM_Initialize();
    SCCP2_PWM_Enable();
    /* Timer SCCP1 odreduje razdoblje upravljackog ciklusa od 5 ms. */
    SCCP1_Timer_Initialize();
    SCCP1_Timer_TimeoutCallbackRegister(TMR_5ms);
    SCCP1_Timer_Start();
    /* UART se koristi za slanje rezultata prema racunalu. */
    UART1_Initialize();
    /* QEI1 obraduje kvadraturne signale inkrementalnog enkodera. */
    QEI1_Initialize();
    QEI1_PhaseInputSwappedSet(true);
    QEI1_Enable();
    /* Registracija prekidnih rutina indeksa, tipki i statusnog ulaza. */
    IO_RD9_SetInterruptHandler(INDEX);
    IO_RE7_SetInterruptHandler(BUTTON);
    IO_RE8_SetInterruptHandler(B_CLEAR);
    IO_RE9_SetInterruptHandler(B_DOWN);
    IO_RE12_SetInterruptHandler(ERROR);
    /* ADC kanali mjere analogne izlaze stvarne brzine i struje. */
    ADC1_Initialize();
    ADC1_ChannelCallbackRegister(ADC_Interrupt);
    ADC1_InterruptEnable();
    ADC1_IndividualChannelInterruptEnable(ACT_Speed);
    ADC1_IndividualChannelInterruptEnable(ACT_Current);
    ADC1_Enable();
    
    while(1)
    {
        /*
         * Upravljacko djelovanje dopusteno je samo ako je sustav omogucen,
         * nema pogreske ni zaustavljanja te je statusni ulaz RE12 u stanju
         * koje postojeca programska logika prepoznaje kao spremnost pogona.
         */
        if(system_enable && !error_flag && !stop_flag && !IO_RE12_GetValue())
        {
            start_flag = true;
        }
        else
        {
            start_flag = false;
        }
        
        /* Nakon zavrsetka pretvorbe oba kanala zaustavlja se ADC okidanje. */
        if(speed_ready && current_ready)
        {
            speed_ready = false;
            current_ready = false;
            ADC1_SoftwareTriggerDisable();
        }
        
        /* Pogreska onemogucuje rad sustava i ukljucuje crvenu LED. */
        if(error_flag)
        {
            system_enable = false;
            button_flag = false;
            IO_RE15_SetHigh();
        }
        
        /* Brisanje pogreske i vracanje sigurnosnih zastavica. */
        if(clear_flag)
        {
            clear_flag = false;
            error_flag = false;
            stop_flag = false;
            system_enable = false;
            IO_RE15_SetLow();
        }
        
        /*
         * Obrada tipke za ukljucivanje ili iskljucivanje.
         * Pri novom pokretanju vraca se pocetni Swing-Up nacin rada.
         */
        if(button_flag && !error_flag)
        {
            button_flag = false;
            last_swing = false;
            down_request = false;
            mode = CTRL_SWING;
            xf = 0.0f;
            system_enable = !system_enable;
            if(system_enable && IO_RE12_GetValue())
            {
                stop_flag = true;
            }
        }
        
        /* Signalizacija omogucenog sustava. */
        if(system_enable)
        {
            IO_RD7_SetHigh();
        }
        else
        {
            IO_RD7_SetLow();
        }
        
        /* Signalizacija zaustavljanja zbog neispravnog stanja pogona. */
        if(stop_flag)
        {
            IO_RE13_SetHigh();
        }
        else
        {
            IO_RE13_SetLow();
        }
        
        /* Signalizacija aktivnog izvodenja upravljackog algoritma. */
        if(start_flag)
        {
            IO_RE14_SetHigh();
        }
        else
        {
            IO_RE14_SetLow();
        }
        
        /* Obrada korisnickog zahtjeva za kontrolirano spustanje. */
        if(down_flag)
        {
            down_flag = false;
            down_request = !down_request;
        }
        
        /* Signalizacija aktivnog zahtjeva za Swing-Down nacin. */
        if(down_request)
        {
            IO_RE6_SetHigh();
        }
        else
        {
            IO_RE6_SetLow();
        }
        
        /* 
         * Indeksni impuls postavlja polozajni brojac na nulu.
         * Prvi izracun brzine nakon resetiranja preskace se kako promjena
         * brojaca ne bi proizvela lazno veliku kutnu brzinu.
         */
        if(index_flag)
        {
            index_flag = false;
            QEI1_PositionCountWrite(0);
            skip_delta = true;
        }
        
        /* Izvodenje jednog mjernog i upravljackog ciklusa svakih 5 ms. */
        if(control_tick)
        {
            control_tick = false;
            
            /* Pokretanje ADC pretvorbe signala brzine i stvarne struje. */
            ADC1_SoftwareTriggerEnable();
            
            /*
             * Pretvorba ADC vrijednosti brzine u okretaje po minuti.
             * Vrijednosti 248 i 3971 odgovaraju naponima 0,2 V i 3,2 V,
             * dok 2109,5 predstavlja sredisnju vrijednost za nultu brzinu.
             */
            //rpm = ((float)adc_speed/2047.5f - 1.0f) *RPM_MAX;
            rpm = ((float)(adc_speed-2109.5f)/((3971.0f-248.0f)/2.0f)) *RPM_MAX;
            /* Pretvorba brzine u rad/s i medijansko filtriranje. */
            omega_w = rpm*(2.0f*PI/60.0f);
            w0 = w1;
            w1 = w2;
            w2 = omega_w;
            omega_w = median(w0, w1, w2);
            
            /* 
             * Pretvorba ADC vrijednosti analognog izlaza stvarne struje
             * u ampere te medijansko filtriranje triju uzoraka.
             */
            //I_act = ((float)adc_current/2047.5f - 1.0f) *I_MAX;
            I_act = ((float)(adc_current-2109.5f)/((3971.0f-248.0f)/2.0f)) *I_MAX;
            I0 = I1;
            I1 = I2;
            I2 = I_act;
            I_act = median(I0, I1, I2);
            
            /* Ocitavanje trenutacnog stanja polozaja brojaca. */
            enc_count = QEI1_PositionCountRead();
            
            if(skip_delta)
            {
                enc_count_prev = enc_count;
                skip_delta = false;
            }
            else
            {
                /* Promjena broja impulsa tijekom jednog ciklusa. */
                delta = enc_count - enc_count_prev;
                enc_count_prev = enc_count;
                /* Izracun kutne brzine tijela. */
                //omega_b=(int32_t)((float)delta*2.0f*PI/(Ts*(float)CPR*4.0f));
                omega_b=(float)delta*W_K;   
            }
            /* Izracun enkoderskog kuta tijela. */
            //theta_enc=(2*PI*(float)enc_count)/(CPR*4);
            theta_enc=(float)enc_count*THETA_K;
            
            /* 
             * Dodavanje pi uskladuje enkodersku i modelnu definiciju kuta.
             * nakon normalizacije nula odgovara gornjem ravnoteznom polozaju.
             */
            theta_norm = NormalizeAngle(theta_enc+PI);
            
            /* Izracun energije i azuriranje trenutacnog nacina rada. */
            Energija(theta_enc+PI, theta_norm, omega_b);
            Switch_Update(theta_norm);
            
            /* Izvodenje regulatora koji pripada trenutacnom nacinu rada. */
            if(start_flag)
            {
                switch(mode)
                {
                    case CTRL_SWING:
                        Swing_Update(omega_b);
                        I_out = MotorLimit(I_swing, omega_w);
                        break;
                    case CTRL_NULA:
                        I_out = 0.0f;
                        break;
                    case CTRL_LQR:
                        OffsetFilterUpdate(theta_norm);
                        LQR_Update(theta_norm, omega_b, omega_w);
                        I_out = MotorLimit(I_lqr, omega_w);
                        break;
                    case CTRL_DOWN:
                        Swing_Down(E, omega_b, omega_w);
                        I_out = MotorLimit(I_down, omega_w);
                        break;
                    default:
                        I_out = 0.0f;
                        break;
                }   
            }
            else
            {
                I_out = 0.0f;
            }
            
            /* 
             * Linearno preslikavanje strujne reference u faktor ispune:
             * -I_MAX odgovara 15 %, nula 50 %, a +I_MAX 85 %.
             */
            float duty_percent = (50.0f + 35.0f*I_out/I_MAX);
            /* Ogranicavanje faktora ispune i pretvorba u SCCP2 broj perioda. */
            duty_percent = Clamp(duty_percent, DUTY_MIN, DUTY_MAX);
            duty_cycle = (uint16_t)(PWM_PERIOD*duty_percent/100.0f);
            SCCP2_PWM_DutyCycleSet(duty_cycle);
            
            /* 
             * UART paket sadrzi cetiri 32-bitne vrijednosti tipa float:
             * enkoderski kut, brzinu tijela, brzinu zamasnjaka i stvarnu struju.
             */
            float tx_data[4];
                
            tx_data[0] = theta_enc;
            tx_data[1] = omega_b;
            tx_data[2] = omega_w;
            tx_data[3] = I_act;
            
            /* Slanje 16 bajtova cetiriju vrijednosti tipa float. */
            uint8_t *ptr = (uint8_t*)tx_data;
            for(uint8_t i = 0; i<16; i++)
            {
                UART1_Write(ptr[i]);
            }
            /* Sedamnaesti bajt paketa predstavlja trenutacni nacin rada. */
            UART1_Write((uint8_t)mode);
            
            /* Pokretanje fizickog UART prijenosa pripremljenog paketa. */
            UART1_TxStart();
        }
    }
}