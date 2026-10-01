**Cubli Control System**



Projekt upravljanja jednoosnim Cubli sustavom izrađen u sklopu diplomskog rada.

Program je napisan u C-u za mikrokontroler dsPIC33CK256MP508.







**Što sustav radi**



Sustav može:

* podići Cubli prema gornjem položaju
* stabilizirati Cubli u gornjem položaju
* kontrolirano spustiti Cubli
* mjeriti položaj i brzinu tijela pomoću inkrementalnog optićkog enkodera
* mjeriti brzinu zamašnjaka i struju motora pomoću pogonskog pretvarača ESCON 50/5
* upravljati motorom pomoću PWM signala
* slati mjerene podatke prema računalu preko UART komunikacije







**Način rada**



Program ima nekoliko načina rada:

* Swing-up - povećava energiju sustava kako bi se Cubli podigao
* Nula - struja na motoru se postavlja na nula
* LQR - stabilizira Cubli u gornjem položaju
* Swing-Down - kontrolirano spušta sustav







**Korišteni alati**



* C
* MPLAB X
* MPLAB Code Configurator
* dsPIC33CK256MP508







**Periferije**



U projektu se koriste:

* QEI za enkoder
* ADC za mjerenje analognih signala
* PWM za upravljanje motorom
* UART za slanje podataka
* timer za periodičko izvođenje upravljačkog ciklusa







**Struktura projekta**



* main.c - glavni program i upravljački algoritmi
* lookup\_table.c i lookup\_table.h - tablica koju koristi Swing-Up algoritam
* mcc\_generated\_files/ - kod za periferije generiran pomoću MCC-a
* nbproject/ - MPLAB X projektne postavke







**Video**



\[Video rada sustava](https://www.youtube.com/watch?v=pOpP5bC\_Jxo)

