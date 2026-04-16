#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "time.h"
#include <WiFi.h>
#include "esp_sntp.h"
#include <ESPmDNS.h>

//--------pin---------
int boutonlumiere=33;
int lampe=15;
int reveil=13;
//--------bouton--------
int etat_bouton;
int etat_precedent;
//---------reveil--------
bool fin_reveil;
int modulo_seconde;
int minute_entree_reveil;
int difference_reveil;
bool faire_reveil=1;

//--------heure-------
int minute;
int heure;
int seconde;
char mois;
int jour;
int annee;
int temp;
int decalage;
int temp_apres;
int difference_22h;
bool commencer_10_minutes=1;
int minute_entree_22h;
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
bool commencer_attendre_10min;

//--------WiFi
const char* ssid = "VOTRE_NOM_DE_BOX";
const char* password = "VOTRE_MOT_DE_PASSE";
//--------WebServer-------
AsyncWebServer serveur(80);  
bool weekend_active_reveil_serveur;
int heure_reveil_serveur;
int minute_reveil_serveur;
bool reveil_active_serveur;
bool lampe_soir_active_serveur;
bool weekend_active_lampe_soir_serveur;
int heure_lampe_soir_serveur;
int minute_lampe_soir_serveur;
char oui_non[2][4]={"non","oui"};
char mon_code_secret[30]="VOTRE_CODE_SECRET";
//--------lumiere--------
bool lumiere_allumee=0;
//------------------------
//---------fonction-------------
  //---------Serveur-----------
    //lampe soir
void heure_plus_lampe_soir(){
  heure_lampe_soir_serveur++;//La variable heure_lampe_soir_serveur ogmente de 1
  if (heure_lampe_soir_serveur>23){//Si heure_lampe_soir_serveur est supérieur à 23 alors :
    heure_lampe_soir_serveur=0;//heure_lampe_soir_serveur vaut 0
  }
}
void heure_moins_lampe_soir(){
  heure_lampe_soir_serveur--;//La variable heure_lampe_soir_serveur diminue de 1
  if (heure_lampe_soir_serveur<0){//Si heure_lampe_soir_serveur est inférieur à 0 alors :
    heure_lampe_soir_serveur=23;//heure_lampe_soir_serveur vaut 23
  }
}
void minute_plus_lampe_soir(){
  minute_lampe_soir_serveur++;//La variable minute_lampe_soir_serveur ogmente de 1
  if (minute_lampe_soir_serveur>59){//Si minute_lampe_soir_serveur est supérieur à 59 alors :
    minute_lampe_soir_serveur=0;//minute_lampe_soir_serveur vaut 0
  }
}
void minute_moins_lampe_soir(){
  minute_lampe_soir_serveur--;//La variable minute_lampe_soir_serveur diminue de 1
  if (minute_lampe_soir_serveur<0){//Si minute_lampe_soir_serveur est inférieur à 0 alors :
    minute_lampe_soir_serveur=59;//minute_lampe_soir_serveur vaut 59
  }
}
void lampe_soir_different(){
  lampe_soir_active_serveur=!lampe_soir_active_serveur;//Si variable lampe_soir_active_serveur vaut 0 alors elle vaut 1 et si elle vaut 1 aolrs elle vaut 0
}
void weekend_different_lumiere(){
  weekend_active_lampe_soir_serveur=!weekend_active_lampe_soir_serveur;//Si variable weekend_active_lampe_soir_serveur vaut 0 alors elle vaut 1 et si elle vaut 1 aolrs elle vaut 0
}
    //reveil
void heure_plus_reveil(){
  faire_reveil=1;//faire_reveil vaut un pour dire que l'on peut relancer le reveil si c'est la bonne heure
  heure_reveil_serveur++;//La variable heure_reveil_serveur ogmente de 1
  if (heure_reveil_serveur>23){//Si heure_reveil_serveur est supérieur à 23 alors :
    heure_reveil_serveur=0;//heure_reveil_serveur vaut 0
  }
  
}
void heure_moins_reveil(){
  faire_reveil=1;//faire_reveil vaut un pour dire que l'on peut relancer le reveil si c'est la bonne heure
  heure_reveil_serveur--;//La variable heure_reveil_serveur diminue de 1
  if (heure_reveil_serveur<0){//Si heure_reveil_serveur est inférieur à 0 alors :
    heure_reveil_serveur=23;//heure_reveil_serveur vaut 23
  }
  
}
void minute_plus_reveil(){
  faire_reveil=1;//faire_reveil vaut un pour dire que l'on peut relancer le reveil si c'est la bonne heure
  minute_reveil_serveur++;//La variable minute_reveil_serveur ogmente de 1
  if (minute_reveil_serveur>59){//Si minute_reveil_serveur est supérieur à 59 alors :
    minute_reveil_serveur=0;//minute_reveil_serveur vaut 0
  }
  
}
void minute_moins_reveil(){
  faire_reveil=1;//faire_reveil vaut un pour dire que l'on peut relancer le reveil si c'est la bonne heure
  minute_reveil_serveur--;//La variable minute_reveil_serveur diminue de 1
  if (minute_reveil_serveur<0){//Si minute_reveil_serveur est inférieur à 0 alors :
    minute_reveil_serveur=59;//minute_reveil_serveur vaut 59
  }
  
}
void reveil_different(){
  faire_reveil=1;//faire_reveil vaut un pour dire que l'on peut relancer le reveil si c'est la bonne heure
  reveil_active_serveur=!reveil_active_serveur;//Si variable reveil_active_serveur vaut 0 alors elle vaut 1 et si elle vaut 1 aolrs elle vaut 0
}
void weekend_different_reveil(){
  faire_reveil=1;//faire_reveil vaut un pour dire que l'on peut relancer le reveil si c'est la bonne heure
  weekend_active_reveil_serveur=!weekend_active_reveil_serveur;//Si variable weekend_active_reveil_serveur vaut 0 alors elle vaut 1 et si elle vaut 1 aolrs elle vaut 0
  
}

  //----------heure
void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}
void initTime(String timezone){
  struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
}
void donner_heure() {
  //Cette fonction sert à récupérer l'heure sur le serveur ntp
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
  jour=timeinfo.tm_wday;
  heure=timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  seconde=timeinfo.tm_sec;

  // Serial.print("minute : ");
  // Serial.println(minute);
  // Serial.print("seconde : ");
  // Serial.println(seconde);
  // Serial.print("heure : ");
  // Serial.println(heure);
  // Serial.print("jour : ");
  // Serial.println(jour);
}
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  donner_heure();
}
  //-----------lumiere-------------
void eteidre_lum(){
  lumiere_allumee=0;//lumiere_allumee vaut 0
  Serial.println("éteint");//On écrit "éteint" dans le moniteur série (ça peut faciliter les débeugages)
  digitalWrite(lampe,HIGH);//On éteit le lumière
}
void allumer_lum(){
  lumiere_allumee=1;//lumiere_allumee vaut 1
  Serial.println("allumé");//On écrit "allumé" dans le moniteur série (ça peut faciliter les débeugages)
  digitalWrite(lampe,LOW);//On allume le lumière
}
  //----------reveil----------------
void reveil_musique(){
  donner_heure();//On appelle la fonction donner_heure pour que l'on soit à jour sur l'horaire
  minute_entree_reveil=minute;//On enregistre la minute d'entrée dans la fonction dans minute_entree_reveil pour pouvoir sortir de la boucle au delas de 2 minutes
  fin_reveil=0;//Mettre fin_reveil à 0 pour que la boucle for ne face pas qu'un tour
  digitalWrite(reveil,LOW);//Le reveil ne fais pas de son
  for(fin_reveil;fin_reveil==0;fin_reveil=fin_reveil){//On ouvre la boucle. Elle va boucler tant que fin_reveil==0 
    donner_heure();//On appelle la fonction donner_heure pour que l'on soit à jour sur l'horaire
    modulo_seconde=seconde%2;//Ici, on regarde si la seconde en cours est paire ou impair. Si elle est paire alors modulo_seconde vaut 0 sinon, modulo_seconde vaut 1
    Serial.println("réveil!!!");////On écrit "réveil!!!" dans le moniteur série (ça peut faciliter les débeugages)
    fin_reveil=0;//Mettre fin_reveil à 0 
    difference_reveil=minute-minute_entree_reveil;//On regare depuis combien de temp on est dans la boucle
    if(difference_reveil<0){//Si difference_reveil est inférieur à 0 (c'est possible si on à changer d'heure ex : on entre à la minute 59 et maintenent on est à la minute 0 0-59=-59) alors
      difference_reveil=60-minute_entree_reveil+minute;//difference_reveil vaut 60-la minute d'entrée plus la minute actuelle(Si on reprend notre exemple, on fait 60-59+0=1)
    }
    if(difference_reveil>=2){//Si ça fait 2 minutes ou plus qu'on est entrer dans la boucle alors :
      fin_reveil=1;//La variable fin_reveil vaut 1 ce qui va nous faire sortir de la boucle
    }
    //Ici on regarde si le bouton est préssé
    etat_precedent = etat_bouton;
    etat_bouton=digitalRead(boutonlumiere);
    if(etat_bouton!=etat_precedent){//regarder si le bouton est pressé
      fin_reveil=1;        
      Serial.println("fin par bouton");
      delay(10);//Je met un délais pour éviter les rebons
    }
    if(modulo_seconde==0){//Si modulo_seconde vaut 0 (la seconde est paire) alors:
      digitalWrite(reveil,HIGH);//Faire biper le buzzer
    }
    else{//Sinpn
      digitalWrite(reveil,LOW);//Faire taire le reveil
    }
  }
  digitalWrite(reveil,LOW);////Faire taire le reveil pour être sur qu'il ne continu pas à biper
  faire_reveil=0;
  allumer_lum();//allumer le lumière (pour un reveil plus éficace :) )
}
//---------------------------------
void setup() {
  //-----------Moniteur série---------------
  Serial.begin(115200);
  Serial.println("\n");
  //-----------GPIO---------------
  pinMode(boutonlumiere,INPUT_PULLUP);//boutonlumiere est une entrée avec une résistance interne
  pinMode(lampe,OUTPUT);//lampe est une sortis
  pinMode(reveil,OUTPUT);//reveil est une sortis
  etat_bouton=digitalRead(boutonlumiere);//etat_bouton=l'état physique du bouton/interrupteur
  etat_precedent=etat_bouton;//etat_precedent=l'état physique du bouton/interrupteur=etat_bouton
  //-----------SPIFFS---------------
  if(!SPIFFS.begin()){//Si SPIFFS n'est pas démaré alors :
    Serial.println("Erreur SPIFFS...");//écrire dans le moniteur série Erreur SPIFFS...
    return;
  }
  //Cette partie de code sert à siter le noom de tous les fichiers contenus dans l'esp
  File root = SPIFFS.open("/");//Ici, on définit root comme racine
  File file = root.openNextFile();//Ici, on ouvre le fichier suivant

  while (file){//tant qu'il y a un nouveau fichier
    Serial.print("File : ");//écrire File : 
    Serial.println(file.name());//puis le nom du fichier
    file.close();//fermer le fichier
    file = root.openNextFile();//Ici, on ouvre le fichier suivant
  }
  
  //-----------WiFi---------------
  Serial.printf("Connecting to %s ", ssid);//écrire Connecting to LE NOM DU ROUTEUR
  WiFi.begin(ssid, password);//initialisation du WiFi
  while (WiFi.status() != WL_CONNECTED) {//tant que l'esp n'est pas conecté alors :
    delay(500);//attendre 500 milliseconde
    Serial.print(".");//puis afficher un point
  }
  Serial.println(" CONNECTED");//écrire CONNECTED
  Serial.print("Adresse IP : ");//écrire Adresse IP : 
  Serial.println(WiFi.localIP());//écrire l'adresse IP de l'esp

  //------------heure--------------
  //initialisation du serveur NTP
  initTime("CET-1CEST,M3.5.0,M10.5.0/3");
  donner_heure();
  //-----------Serveur---------------
  //Ici on initialise le serveur et on précise que faire si on atteint tel endroit dans l'arborécense ou si tel fonction est appelée
  serveur.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/reveil.html", "text/html");
  });
  serveur.on("/reveil.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/reveil.css", "text/css");
  });
  serveur.on("/reveil.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/reveil.js", "text/javascript");
  });
  serveur.on("/mot_de_passe", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(200,"text/plain",mon_code_secret);
  });
  serveur.on("/weekend_r", HTTP_GET, [](AsyncWebServerRequest *request){
    weekend_different_reveil();
    Serial.print("weekend_reveil : ");
    Serial.println(oui_non[weekend_active_reveil_serveur]);
    request->send(200);
  });
  serveur.on("/plus_h_r", HTTP_GET, [](AsyncWebServerRequest *request){
    heure_plus_reveil();
    Serial.print("heure_reveil : ");
    Serial.println(heure_reveil_serveur);
    request->send(200);
  });
  serveur.on("/moins_h_r", HTTP_GET, [](AsyncWebServerRequest *request){
    heure_moins_reveil();
    Serial.print("heure_reveil : ");
    Serial.println(heure_reveil_serveur);
    request->send(200);
  });
  serveur.on("/plus_m_r", HTTP_GET, [](AsyncWebServerRequest *request){
    minute_plus_reveil();
    Serial.print("minute_reveil : ");
    Serial.println(minute_reveil_serveur);
    request->send(200);
  });
  serveur.on("/moins_m_r", HTTP_GET, [](AsyncWebServerRequest *request){
    minute_moins_reveil();
    Serial.print("minute_reveil : ");
    Serial.println(minute_reveil_serveur);
    request->send(200);
  });
  serveur.on("/active_r", HTTP_GET, [](AsyncWebServerRequest *request){
    reveil_different();
    Serial.print("reveil_activé : ");
    Serial.println(oui_non[reveil_active_serveur]);
    request->send(200);
  });
  serveur.on("/weekend_l", HTTP_GET, [](AsyncWebServerRequest *request){
    weekend_different_lumiere();
    Serial.print("weekend_lumiere : ");
    Serial.println(oui_non[weekend_active_lampe_soir_serveur]);
    request->send(200);
  });
  serveur.on("/plus_h_l", HTTP_GET, [](AsyncWebServerRequest *request){
    heure_plus_lampe_soir();
    Serial.print("heure_lampe : ");
    Serial.println(heure_lampe_soir_serveur);
    request->send(200);
  });
  serveur.on("/moins_h_l", HTTP_GET, [](AsyncWebServerRequest *request){
    heure_moins_lampe_soir();
    Serial.print("heure_lampe : ");
    Serial.println(heure_lampe_soir_serveur);
    request->send(200);
  });
  serveur.on("/plus_m_l", HTTP_GET, [](AsyncWebServerRequest *request){
    minute_plus_lampe_soir();
    Serial.print("minute_lampe : ");
    Serial.println(minute_lampe_soir_serveur);
    request->send(200);
  });
  serveur.on("/moins_m_l", HTTP_GET, [](AsyncWebServerRequest *request){
    minute_moins_lampe_soir();
    Serial.print("minute_lampe : ");
    Serial.println(minute_lampe_soir_serveur);
    request->send(200);
  });
  serveur.on("/active_l", HTTP_GET, [](AsyncWebServerRequest *request){
    lampe_soir_different();
    Serial.print("lampe_activé : ");
    Serial.println(oui_non[lampe_soir_active_serveur]);
    request->send(200);
  });



  Serial.println(digitalRead(boutonlumiere));






  //--------------au démarage------------





  serveur.on("/weekend_r_s", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.print("weekend_reveil : ");
    Serial.println(oui_non[weekend_active_reveil_serveur]);
    request->send(200,"text/plain",String(weekend_active_reveil_serveur));
  });
  serveur.on("/h_r_s", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.print("heure_reveil : ");
    Serial.println(heure_reveil_serveur);
    request->send(200,"text/plain", String(heure_reveil_serveur));
  });
  
  serveur.on("/m_r_s", HTTP_GET, [](AsyncWebServerRequest *request){
    
    Serial.print("minute_reveil : ");
    Serial.println(minute_reveil_serveur);
    request->send(200,"text/plain", String(minute_reveil_serveur));
  });
  
  serveur.on("/active_r_s", HTTP_GET, [](AsyncWebServerRequest *request){
    
    Serial.print("reveil_activé : ");
    Serial.println(oui_non[reveil_active_serveur]);
    request->send(200,"text/plain", String(reveil_active_serveur));
  });
  serveur.on("/weekend_l_s", HTTP_GET, [](AsyncWebServerRequest *request){
    
    Serial.print("weekend_lumiere : ");
    Serial.println(oui_non[weekend_active_lampe_soir_serveur]);
    request->send(200,"text/plain",String(weekend_active_lampe_soir_serveur));
  });
  serveur.on("/h_l_s", HTTP_GET, [](AsyncWebServerRequest *request){
    
    Serial.print("heure_lampe : ");
    Serial.println(heure_lampe_soir_serveur);
    request->send(200,"text/plain", String(heure_lampe_soir_serveur));
  });
  
  serveur.on("/m_l_s", HTTP_GET, [](AsyncWebServerRequest *request){
    
    Serial.print("minute_lampe : ");
    Serial.println(minute_lampe_soir_serveur);
    request->send(200,"text/plain", String(minute_lampe_soir_serveur));
  });
  
  serveur.on("/active_l_s", HTTP_GET, [](AsyncWebServerRequest *request){
    
    Serial.print("lampe_activé : ");
    Serial.println(oui_non[lampe_soir_active_serveur]);
    request->send(200,"text/plain", String(lampe_soir_active_serveur));
  });
  //penser a continuer pour toute les fonctions et a copier les fonctions de l encien code
  serveur.begin();
  Serial.println("Serveur actif");
  //------------MDNS--------------
  MDNS.begin("parametre_reveil");
  MDNS.addService("http","tcp",80);
  //------------------------------
}

void loop() {
  // put your main code here, to run repeatedly:
  donner_heure(); //fonction pour avoir l'heure
  if (lumiere_allumee){//Si la lumière est allumée alors
    if(lampe_soir_active_serveur){//Si éteindre lampe soir est à "Oui" sur le site internet
      if ((heure==heure_lampe_soir_serveur && minute>=minute_lampe_soir_serveur) || heure>heure_lampe_soir_serveur){//regarder si l'heure actuelle est supérieure ou égale à l'heure choisie sur le site
        if(weekend_active_lampe_soir_serveur){//Si dans Lampe soir week-end est à oui sur le serveur
          commencer_attendre_10min=1;//on commence à attendre 10 minute
        }
        else{//Si dans Lampe soir week-end est à non sur le serveur
          if(jour==1 or jour==2 or jour==3 or jour==4 or jour==5){//Si on est en semaine
            commencer_attendre_10min=1;//on commence à attendre 10 minute
          }
        }
      }  
    }
  }
  if(commencer_attendre_10min){//Si on à dit au paravent qu'on commence les 10 minutes alors :
    //Ici, c'est un peu comme si on fesait un chronometre
    if(commencer_10_minutes){//Si les 10 minutes ne sont pas encore en cours alors :
      commencer_10_minutes=0;//On dit de ne pas recommencer cette condition
      minute_entree_22h=minute;//On précise l'heure d'entrée
    }
    difference_22h=minute-minute_entree_22h;//On regarde depuis cobien de temp le "chronometre" est lancé
    if(difference_22h<0){//Si difference_22h est inférieur à 0 alors
      difference_22h=60-minute_entree_22h+minute;//difference_reveil vaut 60-la minute d'entrée plus la minute actuelle
    }
    if(difference_22h>=10){//Si ça fait 10 minute que le "chronometre" à été lancer alors :
      eteidre_lum();//éteindre la lumiere
      commencer_10_minutes=1;//On dit que le chronometre est terminé
      commencer_attendre_10min=0;//On dit que le chronometre est terminé
    }
  }
  if(heure==0 and minute==0 and seconde==0){//Si il est minuit alors
    faire_reveil=1;//On peut lancer le reveil si c'est l'heure
  }
  //On regarde si le bouton est présser
  etat_precedent = etat_bouton;
  etat_bouton=digitalRead(boutonlumiere);  
  if(etat_bouton!=etat_precedent){//regarder si le bouton est pressé
      lumiere_allumee = !lumiere_allumee;//changer la valeur d'état lumiere
      if (lumiere_allumee){//Si on doit allumer la lumière
        allumer_lum();//appeler allumer lum
      }
      else{//sinon :
        eteidre_lum();//appeler eteindre lum
      }
      delay(10);//Un petit délais pour éviter les rebonds
      
  }
  if(reveil_active_serveur){//Si dans reveil reveil est à oui sur le serveur
    if(weekend_active_reveil_serveur){//Si dans reveil week-end est à oui sur le serveur
      if(heure==heure_reveil_serveur){//Si l'heure actuelle est la même que celle renseigné dans la page web
        if(minute==minute_reveil_serveur){//Si la minute actuelle est la même que celle renseigné dans la page web
          if(faire_reveil==1){//Et si le réveil n'a pas été déjas fait dans cette journée
            reveil_musique();//alors activer le reveil
          }
        }
      }
    }
    else{
      if(jour==1 or jour==2 or jour==3 or jour==4 or jour==5){//Si on est en semaine
        if(heure==heure_reveil_serveur){//Si l'heure actuelle est la même que celle renseigné dans la page web
          if(minute==minute_reveil_serveur){//Si la minute actuelle est la même que celle renseigné dans la page web
            if(faire_reveil==1){//Et si le réveil n'a pas été déjas fait dans cette journée
              reveil_musique();//alors activer le reveil
            }
          }
        }   
      }
    }   
  }
}

