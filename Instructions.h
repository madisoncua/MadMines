typedef enum {English, Spanish} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";

const char Page1_English[] =  "Controls:\n\n"
                              "Left (Low) Button:\n"
                              "Put down/Pick up\n\n"
                              "Right (High) Button:\n"
                              "Interact with\n"
                              "machine\n\n"
                              "Joystick:\n"
                              "Movement\n\n"
                              "(Click right for\n"
                              "more instructions,\n" 
                              "click left to go\n"
                              "back.)";

const char Page2_English[] = "Your Mission:\n\n"
                              "The 319H Professors\n"
                              "are embarking on a \n"
                              "journey.\n\n"
                              "It is your job to\n"
                              "supply them with the\n"
                              "magical items they\n"
                              "need before time\n"
                              "runs out.\n\n"
                              "Don't make something\n"
                              "wrong...you might\n"
                              "get something\n"
                              "unexpected!";

const char Page3_English[] =  "You can't do this\n"
                              "alone. This is a\n"
                              "cooperative game.\n\n"
                              "Use the cart to\n"
                              "transfer items\n"
                              "between you and\n"
                              "your partner.\n"
                              "Work together to\n"
                              "make the completed\n"
                              "items.\n\n"
                              "Remember, only one\n"
                              "person has the\n"
                              "to do list!\n";

const char Page4_English[] = "How To Use:\n\n"
                              "Anvil:\n"
                              "Left button:\n"
                              " - open menu\n"
                              " - pick up/put down\n"
                              "Right button:\n"
                              " - put items in menu\n"
                              " - make item\n\n"
                              "To Do Menu:\n"
                              "Right Button:\n"
                              " - opens/closes to\n"
                              "do\n";
                            
const char Page5_English[] = "How To Use (cont.):\n\n"
                              "Smelter:\n"
                              "Flashing indicates\n"
                              "time is up!\n\n"
                              "Let's Play!\n"
                              "Click right button\n"
                              "to begin";

const char Page1_Spanish[] = "Controles:\n"
                              "Bot\xA2n izquierdo:\n" 
                              "Dejar/recoger\n\n"
                              "Bot\xA2n derecho:\n"
                              "Interactuar con\n"
                              "la m\xA0quina\n\n"
                              "(clic a la derecha\n"
                              "para m\xA0s\n"
                              "instrucciones\n" 
                              "clic a la\n"
                              "izquierda para\n"
                              "volver atr\xA0s)";

const char Page2_Spanish[] =  "Controles (cont.):\n"
                              "Joystick:\n"
                              "Movimiento\n\n"
                              "Tu misi\xA2n:\n"
                              "Los Profesores de\n"
                              "319H se embarcan\n"
                              "en un adventura.\n"
                              "Tu trabajo es\n"
                              "proporcionarles\n"
                              "los objetos\n"
                              "m\xA0gicos que\n"
                              "necesitan antes\n"
                              "dar le se acabe\n"
                              "el tiempo.";

const char Page3_Spanish[] = "No cometas\n"
                              "errores...\n"
                              "\xADpodrías obtener\n"
                              "algo inesperado!\n\n"
                              "No puedes hacerlo\n"
                              "solo. Este es un\n"
                              "juego cooperativo.\n"
                              "Usa el carrito para\n"
                              "transferir objectos\n"
                              "entre t\xA3 y tu\n"
                              "compa\xA4""ero.\n\n"
                              "Trabajen juntos\n"
                              "para completar\n"
                              "los objetos";

const char Page4_Spanish[] = "Recuerda,\n\n"
                              "\xADsolo una:\n"
                              "persona tiene\n"
                              "la lista de\n"
                              "tareas!\n\n"
                              "C\xA2mo usar:\n"
                              "Yunque:\n"
                              "- abrir men\xA3\n"
                              "- recoger/dejar\n"
                              "Bot\xA2n derecho:\n"
                              "- poner objetos en\n"
                              "el men\xA3\n"
                              "- crear objeto";

const char Page5_Spanish[] = "Men\xA3 de tareas:\n\n"
                              "Bot\xA2n derecho:\n"
                              "- abrir/cerrar para\n"
                              "hacer\n\n"
                              "Fundici\xA2n:\n"
                              "Parpadeando\n"
                              "indica que\n"
                              "\xADse acab\xA2\n"
                              "el tiempo!\n"
                              "\xAD""A jugar!\n"
                              "clic derecho\n"
                              "para empezar";

const char Won_English[] = "YOU WON!\n"
                            "                FINAL SCORE: \n";

const char Lost_English[] = "YOU LOST!\n"
                            "                FINAL SCORE: \n";

const char Won_Spanish[] = "GANASTE!\n"
                            "                PUNTUACIÓN FINAL: \n";

const char Lost_Spanish[] = "PERDISTE!\n"
                            "                PUNTUACI\xA2N FINAL: \n";



const char *Phrases[5][2]={
  {Hello_English,Hello_Spanish},
  {Goodbye_English,Goodbye_Spanish},
  {Language_English,Language_Spanish},
  {Won_English, Won_Spanish},
  {Lost_English, Lost_Spanish}
};

const char *Instructions[2][5]{
  {Page1_English, Page2_English, Page3_English, Page4_English, Page5_English},
  {Page1_Spanish, Page2_Spanish, Page3_Spanish, Page4_Spanish, Page5_Spanish},
};
