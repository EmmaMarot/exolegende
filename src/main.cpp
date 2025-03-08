#include "gladiator.h"
#include "behavior.cpp"
Gladiator *gladiator;
Behavior *behavior;
void reset();
void setup()
{
    // instanciation de l'objet gladiator
    gladiator = new Gladiator();
    // enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset); // GFA 4.4.1
}

void reset()
{
    // fonction de reset:
    // initialisation de toutes vos variables avant le début d'un match
    gladiator->log("Call of reset function"); // GFA 4.5.1
    behavior = new Behavior(gladiator);
    behavior->MC->reset();
}

void loop()
{
    if (gladiator->game->isStarted())
    { // tester si un match à déjà commencer
        // code de votre stratégie   
        behavior->process();
        // gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.6); //control de la roue gauche
        // gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.6); //controle de la roue droite

    }
    else
    {
        // gladiator->log("Hello world - Game not Startd yet"); // GFA 4.5.1
    }
    delay(25);
}
