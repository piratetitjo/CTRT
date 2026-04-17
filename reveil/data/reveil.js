let heure_reveil=0;
let minute_reveil=0;
let reveil_active=0;
let lampe_soir_active=0;
let weekend_active_lampe_soir=0;
let heure_lampe_soir=0;
let minute_lampe_soir=0;
let weekend_active_reveil=0;
let oui_non=['non','oui'];
let mon_code_secret="Joan";
let reponse;
let etat_zonne_texte=1;
let code_juste=false;
// true pour face, false pour dos.

let face= true;
function afficher_mot_de_passe(){
    let checkbox = document.getElementById('voir_mot_de_passe');
    let zone_de_texte = document.getElementById('zone_mot_de_passe');
    
    etat_zonne_texte=!etat_zonne_texte;
    if(checkbox.checked===true){
        zone_de_texte.type="text";
    }
    else{
        zone_de_texte.type="password";
    }
}
// rotation effectuée en ajoutant la class rota.
function rotation3D() {
    if(code_juste){
        let zone = document.getElementById('zoneRota');
        let zone_de_texte = document.getElementById('zone_mot_de_passe');
        let bouton = document.getElementById('bouton');
        let check = document.getElementById('voir_mot_de_passe');
        if (face) {
            zone.className = 'rota';
            zone_de_texte.style.display = 'none';
            bouton.style.display = 'none';
            check.style.display = 'none';
            zone_de_texte.value='';
        }

        else {
            zone.className = '';
            zone_de_texte.style.display = 'inline';
            bouton.style.display = 'inline';
            check.style.display = 'inline';
        }
        face = !face;
        setup();
    }
    else{
        alert("gruge détecté");
    }
}
function verifier_code(){
    let zone_de_texte_valeur = document.getElementById('zone_mot_de_passe').value;
    var xhttp = new XMLHttpRequest();
    
    
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            if(zone_de_texte_valeur==reponse){
                code_juste=true;
                rotation3D();

            }
            reponse="gruge détectée";   
        }
    };
    xhttp.open("GET", "mot_de_passe", true);
    xhttp.send();
    code_juste=false;
    
}
            
// let body=document.getElementsByTagName("body");
// let r_minute =document.getElementById("r_minute");
// let r_active=document.getElementById("r_active");
// let l_weekend=document.getElementById("l_weekend");
// let l_heure=document.getElementById("l_heure");
// let l_minute =document.getElementById("l_minute");
// let l_active=document.getElementById("l_active");

function setup(){
    let r_heure=document.getElementById("r_heure");
    let r_weekend=document.getElementById("r_weekend");
    let body=document.getElementsByTagName("body");
    let bouton=document.getElementsByTagName("a");
    let r_minute =document.getElementById("r_minute");
    let r_active=document.getElementById("r_active");
    let l_weekend=document.getElementById("l_weekend");
    let l_heure=document.getElementById("l_heure");
    let l_minute =document.getElementById("l_minute");
    let l_active=document.getElementById("l_active");
    var xhttp = new XMLHttpRequest();
    
    
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            weekend_active_reveil=reponse;
            r_weekend.innerText=oui_non[Number(reponse)];            
        }
    };
    xhttp.open("GET", "weekend_r_s", true);
    xhttp.send();


    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            heure_reveil=reponse;
            r_heure.innerText=reponse;
            
            
        }
    };
    xhttp.open("GET", "h_r_s", true);
    xhttp.send();


    


    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            minute_reveil=reponse
            r_minute.innerText=reponse;
            
        }
    };
    xhttp.open("GET", "m_r_s", true);
    xhttp.send();






    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            reveil_active=reponse;
            r_active.innerText=oui_non[Number(reponse)];
            
        }
    };
    xhttp.open("GET", "active_r_s", true);
    xhttp.send();




    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            weekend_active_lampe_soir=reponse;
            l_weekend.innerText=oui_non[Number(reponse)];
            
        }
    };
    xhttp.open("GET", "weekend_l_s", true);
    xhttp.send();




    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            heure_lampe_soir=reponse;
            l_heure.innerText=reponse;
            
        }
    };
    xhttp.open("GET", "h_l_s", true);
    xhttp.send();


    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            minute_lampe_soir=reponse;
            l_minute.innerText=reponse;
            
        }
    };
    xhttp.open("GET", "m_l_s", true);
    xhttp.send();




    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function (){
        if (this.readyState ==4 && this.status==200){
            reponse=this.responseText;
            lampe_soir_active=reponse;
            l_active.innerText=oui_non[Number(reponse)];
        }
    };
    xhttp.open("GET", "active_l_s", true);
    xhttp.send();
}
function change_value_weekend_reveil(){
    weekend_active_reveil=!Number(weekend_active_reveil);
    let r_weekend=document.getElementById("r_weekend");
    r_weekend.innerText=oui_non[Number(weekend_active_reveil)];
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "weekend_r", true);
    xhttp.send();
}  
function ogmenter_heure_reveil(){
    heure_reveil++;
    if(heure_reveil>23){
        heure_reveil=0;
    }
    let r_heure=document.getElementById("r_heure");

    r_heure.innerText=heure_reveil;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "plus_h_r", true);
    xhttp.send();
}
function diminuer_heure_reveil(){
    heure_reveil--;
    if(heure_reveil<0){
        heure_reveil=23;
    }
    let r_heure=document.getElementById("r_heure");
    r_heure.innerText=heure_reveil;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "moins_h_r", true);
    xhttp.send();
}
function ogmenter_minute_reveil(){
    minute_reveil++;
    if(minute_reveil>59){
        minute_reveil=0;
    }
   let r_minute =document.getElementById("r_minute");

    r_minute.innerText=minute_reveil;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "plus_m_r", true);
    xhttp.send();
}
function diminuer_minute_reveil(){
    minute_reveil--;
    if(minute_reveil<0){
        minute_reveil=59;
    }
    let r_minute =document.getElementById("r_minute");

    r_minute.innerText=minute_reveil;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "moins_m_r", true);
    xhttp.send();
}
function change_value_active_reveil(){
    reveil_active=!Number(reveil_active);
    let r_active=document.getElementById("r_active");
    r_active.innerText=oui_non[Number(reveil_active)];
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "active_r", true);
    xhttp.send();
}
function change_value_weekend_lampe(){
    weekend_active_lampe_soir=!Number(weekend_active_lampe_soir);

    let l_weekend=document.getElementById("l_weekend");

    l_weekend.innerText=oui_non[Number(weekend_active_lampe_soir)];
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "weekend_l", true);
    xhttp.send();
}
function ogmenter_heure_lampe(){
    heure_lampe_soir++;
    if(heure_lampe_soir>23){
        heure_lampe_soir=0;
    }

    let l_heure=document.getElementById("l_heure");

    l_heure.innerText=heure_lampe_soir;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "plus_h_l", true);
    xhttp.send();
}
function diminuer_heure_lampe(){
    heure_lampe_soir--;
    if(heure_lampe_soir<0){
        heure_lampe_soir=23;
    }

    let l_heure=document.getElementById("l_heure");

    l_heure.innerText=heure_lampe_soir;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "moins_h_l", true);
    xhttp.send();
}
function ogmenter_minute_lampe(){
    minute_lampe_soir++;
    if(minute_lampe_soir>59){
        minute_lampe_soir=0;
    }
    let l_minute =document.getElementById("l_minute");
    l_minute.innerText=minute_lampe_soir;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "plus_m_l", true);
    xhttp.send();
}
function diminuer_minute_lampe(){
    minute_lampe_soir--;
    if(minute_lampe_soir<0){
        minute_lampe_soir=59;
    }
;
    let l_minute =document.getElementById("l_minute");

    l_minute.innerText=minute_lampe_soir;
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "moins_m_l", true);
    xhttp.send();
}
function change_value_active_lampe(){
    lampe_soir_active=!Number(lampe_soir_active);
    let l_active=document.getElementById("l_active");
    l_active.innerText=oui_non[Number(lampe_soir_active)];
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "active_l", true);
    xhttp.send();
}
// setInterval(setup, 5000);