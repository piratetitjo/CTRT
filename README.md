# <div style="text-align:center">Reveil</div>
# :warning: **<span style="color : red"> Ce montage contient du 220V</span>**
<!-- [Comment ça fonctionne](#comment-ça-fonctionne-)\
[Au démarage](#au-démarage-)\
[Une fois le programe uplodé](#une-fois-le-programcdze-uplodé-)\
[Comment utiliser le site internet ?](#comment-utiliser-le-site-internet-)\
[Comment utiliser le reveil et la lampe ?](#comment-utiliser-le-reveil- sc et-la-lampe-)\
[Shéma de branchementvdfv :](#shéma-de-branchement-)\
[Sources](#sources--)\ -->À quoi sert ce programe pour le mode réveil :
- Il sert à mettre un **réveil** grâce à un site internet
- Il sert également à **éteindre une lumière** branchée sur secteur à l'heure voulue
- Il sert aussi à allumer et éteindre la lampe de chevet

À quoi sert ce programe pour le mode radiateur:
- Il sert à programer un radiateur d'apoint grâce à un site internet
- Il sert également à éteindre ou allumer manuellement le radiateur

## Comment installer le code :
- commmencez par clonner le code. Pour ce faire, allez dans un terminal puis tapez ```git clone git@github.com:piratetitjo/CTRT.git```
- Ensuite regardez cette vidéo explicative pour savoir comment uploader les fichiers dans l'ESP32 : [Vidéo SPIFFS](https://www.youtube.com/watch?v=DDMQbqPUliY&t=2004s)
- Une fois ceci fait, allez à la ligne [46, 47 du fichier ```radiateur/src/main.cpp```](https://github.com/piratetitjo/reveil/blob/0e7207493eaafd5bacd40415de36821b74fdc24d/radiateur/src/main.cpp#L46) pour le radiateur et [43, 44](https://github.com/piratetitjo/reveil/blob/f4e3323a008d3412e54e5bfd9000a178378d66a8/reveil/src/main.cpp#L44) pour le réveil puis inscrivez le nom de votre réseau dans la variable ```SSID``` et le mot de passe de votre réseau dans la variable ```password```.
- Allez à la ligne [59](https://github.com/piratetitjo/reveil/blob/f4e3323a008d3412e54e5bfd9000a178378d66a8/radiateur/src/main.cpp#L59) pour le radiateur et [56](https://github.com/piratetitjo/reveil/blob/f4e3323a008d3412e54e5bfd9000a178378d66a8/reveil/src/main.cpp#L56) pour le réveil et inscrivez le mot de passe que vous avez choisis pour pouvoir paramétrer le réveil ou le radiateur dans la variable ```mon_code_secret```
- Munissez vous d'un ordinateur connecté au même réseau que l'ESP32 et taper dans la barre d'adresse d'un moteur de recherche ```parametre_reveil.local``` et inscrivez le mot de passe renseigné précédement dans la variable ```mon_code_secret``` dans l'espace prévu à cet effet.
- Enfin, vous pouvez régler le réveil ou le radiateur grâce aux + et aux -.
## Comment ça fonctionne ?
- ### Au démarrage :
    L'ESP32 indique 4 choses importantes sur le moniteur série:
    -  les noms des fichiers enregistrés dans l'ESP :\
    ```File : reveil.html``` ou ```File : radiateur.html``` pour le mode radiateur\
    ```File : reveil.css``` ou ```File : radiateur.css``` pour le mode radiateur\
    ```File : reveil.js``` ou ```File : radiateur.js``` pour le mode radiateur
    -  Le message qui nous dit qu'on est bien connecté : ```Connecting to Le_NOM_DE_VOTRE_RÉSAU ..... CONNECTED```
    -  l'adresse IP : ```Adresse IP : VOTRE_ADRESSE_IP```
    -  le message qui nous dit que l'heure est bien récupérée : ```Got time adjustment from NTP!```
- ### Une fois le programe uplodé : 
    - Nous avons accés au site internet en entrant l'adresse IP vue en haut sur la barre de recherche d'un navigateur ou de taper ```parametre_reveil.local``` :warning:, pour l'instant, je n'arrive pas à utiliser le site avec un téléphone, il faut un ordinateur.
- ### Comment utiliser le site internet ?
    - Commencez par entrer votre code dans la zone de texte (le code est stocké dans la variable ```mon_code_secret```), puis appuyez sur ```envoyer mot de passe``` 
    > Astuce : vous pouvez voir ce que vous avez écrit comme code en appuyant sur la coche.

    ### **:warning: Le code n'est pas protégé, il est facile pour une autre personne déjà connectée sur le réseau de le récupérer. Ici, c'était plus pour le défi que pour la sécurisation !!!**
    Pour le réveil :
    - la page est sectionnée en deux parties :\
    La partie réveil et la partie lampe soir. Les deux fonctionnent de la même façon :
        - En haut, nous pouvons dire si le reveil ou la lampe est activée.
        - Ensuite nous pouvons renseigner les heures.
        - Ensuite nous pouvons renseigner les minutes.
        - Et enfin nous pouvon dire si le réveil ou la lampe sont activés.

    Pour le radiateur :
     - la page est sectionnée en trois parties :\
    La partie où on dit si le radiateur automatique est activé ou pas, la où on précise l'heure de démarage et la partie où on précise l'heure d'instinction. Les tois fonctionnent de la même façon : il suffit de modifier l'heure ou les autres parametres avec le plus et le moins.

    Une fois que vous avez inscrit les informations, vous pouvez cliquer sur retour et vous retournerez à la page d'accueil.

- ### Comment utiliser le réveil et la lampe ?
    - Pour allumer ou éteindre la lumière, changez l'interrupteur de valeur.
    - Pour que le réveil arrête de sonner, il faut aussi changer l'interrupteur de valeur.

- ### Comment utiliser le radiateur ?
    - Pour allumer ou éteindre le radiateur, changez l'interrupteur de valeur.

- ### Schéma de branchement :
    ![](/shema_des_broches.png "Shema des roches")
- ## Sources  : 
    Ce projet à été réalisé grace aux vidéos de la chaine Tommy Desrochers.\
    Lien vers sa chaine : [Tommy Desrochers](https://www.youtube.com/@TD72PRO)\
    Lien vers sa vidéo sur le SPIFFS : [Vidéo SPIFFS](https://www.youtube.com/watch?v=DDMQbqPUliY&t=2004s)\
    Merci à lui.\
    [Tutoriel module relais](https://www.moussasoft.com/relais-avec-arduino/)
## Variante :
Je me suis rendu compte que cela pouvait m'aider pour l'hiver dans ma chambre, lorsqu'on installait un chauffage d'appoint. Je peux installer le même montage que sur le TITRE en enlevant juste le buzzer et à la place de la lampe, mettre le radiateur. J'ai un petit peu modifié le code pour qu'il puisse s'allumer et s'éteindre à une heure dite. À l'avenir, j'aimerais installer un thermomètre pour qu'il arrête de chauffer à partir d'une température donnée.
## Lien vers les produits utilisés :
- [ESP32](https://www.atelierdelarobotique.fr/produit/carte-de-developpement-esp32-devkitc-avec-module-esp32-wroom-32-wifi-et-bluetooth-integres-compatible-arduino)
- [Module relais 3V3](https://www.cdiscount.com/bricolage/electricite/module-relais-3-1-module-relais-accessoires-de-con/f-1661416-vvi1688442892586.html#mpos=1|mp) :warning: Bien isoler les soudures apparentes avant de le brancher !!! Ne pas poser sur une surface conductrice !!!
- [Buzzer](https://www.cdiscount.com/bricolage/securite-domotique/buzzer-alarme-actif-type-ci/f-1662001-aabpx84658.html#mpos=3|mp)
- [Swich](https://www.cdiscount.com/bricolage/electricite/interrupteur-electronique-nkk/f-1661416-aaaan73002.html#mpos=25|mp)
