# Kleine Anleitung zur Benutzung von Git mit GitAhead

Um Git ein wenig einfacher zu machen ist GitAhead sinnvoll...
GitAhead gibt es für Windows Linux und Mac

## Einrichtung GitAhead

### Token erstellen

 - Damit GitAhead auf unseren GitLab Account zugreifen kann, benötigen wir einen Token welchen wir anstelle eines Passworts in GitAhead benutzen können

 <p align="center">
 <img src="img/img_1.png" alt="" margin="auto" width="500" />
 </p>

 - wichtig: den token gleich kopieren (man sieht den nur einmal)

### GitAhead anmelden

 - Wenn GitAhead gestartet ist, kann man den Token wie gezeigt verwenden:

 <p align="center">
 <img src="img/img_6.png" alt="" height="500" margin="auto" />
 </p>

 - hier die TH-Email, und den Token angeben.
 - auch muss die Server URL angepasst werden um den GitLab Server der TH zu verwenden

 <p align="center">
 <img src="img/img_7.png" alt="" height="200" margin="auto" />
 </p>

### SSH Keys

 - Als nächstes benötigen wir einen SSH Key um auf dem Server hochzuladen und runterzuladen
 - Der Key kann über Putty generiert werden oder über "ssh-keygen"
 - Am Einfachsten ist aber über MSYS2 (da wir das eh schon benutzen). Mit MSYS ist nämlich auch unter Windows der UNIX-Befehl "ssh-keygen" möglich.

 <p align="center">
 <img src="img/img_2.png" alt="" width="500" margin="auto" />
 </p>

 - wenn nach einer passphrase gefragt wird kann man diese auch einfach lehr lassen ;-)

 - Der Public key ist in "id_rsa.pub" die Datei kann man einfach mit einem Editor öffnen und den kompletten Inhalt kopieren

 <p align="center">
 <img src="img/img_3.png" alt="" width="500" margin="auto" />
 </p>

 - Den Key in Gitlab einfügen

 <p align="center">
 <img src="img/img_4.png" alt="" width="500" margin="auto" />
 </p>

 - Damit auch GitAhead den Key kennt muss der Pfad für das keypair angegeben werden (unter Extras->Optionen->Sonstiges)

 <p align="center">
 <img src="img/img_5.png" alt="" width="500" margin="auto" />
 </p>

## Repository Klonen

 - ist GitAhead eingerichtet kann das Repo geklont werden

 - dazu muss der link den Repos von der entsprechenden Git-Page kopiert werden

 - es wird der Link für "Clone with SSH" benötigt

 <p align="center">
 <img src="img/img_8.png" alt="" width="500" margin="auto" />
 </p>

 - dann kann das Repo wie in den Screenshots geklont werden
 - (den Speicherort am besten anpassen) (und merken)

 <p align="center">
 <img src="img/img_9.png" alt="" width="500" margin="auto" />
 </p>

 <p align="center">
 <img src="img/img_10.png" alt="" width="400" margin="auto" />
 </p>

 <p align="center">
 <img src="img/img_11.png" alt="" width="400" margin="auto" />
 </p>

## Entwicheln eines Features

 - Da wir mit Branches arbeiten wollen wird es hier ein wenig schwieriger als nötig
 - Der Zweck von branches ist es immer eine lauffähige Version auf "main" zu haben

### Wtf ist ein Branch

 > - Ein Branch ist eine Kopie des Codes, die von einem Haupt-Code-Repository erstellt wird. 
 > - Das Erstellen eines Branches ermöglicht es dir, Änderungen am Code vorzunehmen, ohne den Haupt-Code zu beeinträchtigen. 
 > - Wenn du deine Arbeit auf einem Branch abgeschlossen hast, kannst du deine Änderungen in den Haupt-Code-Repository (auch als "Merge" oder "Merge Request" bezeichnet) einfügen, damit sie in den Haupt-Code integriert werden können.
 > 
 >   _~ ChatGPT_

 - Am besten stimmen die Branches mit den Aufgaben auf dem Jira-Board überein

### Erstellen eines Branches

 - Um alle Branches angezeigt zu bekommen muss man zuerst auf das Dropdown Menü klicken

 <p align="center">
 <img src="img/img_12.png" alt="" width="500" margin="auto" />
 </p>

 - Mit Doppelklick auf "main" sicherstellen das der main branch der aktuelle ausgecheckte branch ist.

 <p align="center">
 <img src="img/img_13.png" alt="" width="500" margin="auto" />
 </p>

 - Der Aktuelle branch ist immer der grün markierte.

 - Achtung: der aktuell augecheckte branch lässt sich nicht ändern wenn noch uncommited changes vorhanden sind

 - Anschließend kann noch der aktuelle Stand des main branchs heruntergeladen also gepullt werden

 - Jetzt kann von main ein neuer branch erstellt werden (also rechtsklick auf den letzten commit und dann "Neuer Branch") (ab diesem Punkt wird eine Kopie von main erstellt)

 <p align="center">
 <img src="img/img_14.png" alt="" width="500" margin="auto" />
 </p>

 <p align="center">
 <img src="img/img_15.png" alt="" width="300" margin="auto" />
 </p>

 - Der neue branch ist jetzt ausgewählt und kann bearbeitet werden

 - Sind Änderungen gemacht können diese commited werden

 - Wenn man links auf "Uncommittete Änderungen" klickt sieht man rechts alle gemachten Änderungen

 - Durch anklicken der Checkboxes werden die Änderungen ausgewählt die commited werden sollen (gestaged)

 - Dann kann ein Commit-Message eingegeben werden und mit "Commit" bestätigt werden

 <p align="center">
 <img src="img/img_16.png" alt="" width="500" margin="auto" />
 </p>

 <p align="center">
 <img src="img/img_17.png" alt="" width="500" margin="auto" />
 </p>

 - Hier wird als Beispiel eine weitere Änderung gemacht und commited

 <p align="center">
 <img src="img/img_18.png" alt="" width="500" margin="auto" />
 </p>

 - Wenn alle Änderungen commited sind kann der branch gepusht werden

 <p align="center">
 <img src="img/img_19.png" alt="" width="500" margin="auto" />
 </p>

 - Beim Puschen kann ein Fehler angezeigt werden, dass kein remote branch gefunden wurde

 - Durch anklicken des obernen Links kann der branch auf den remote gepusht werden, wobei der remote branch automatisch erstellt wird

 <p align="center">
 <img src="img/img_20.png" alt="" width="500" margin="auto" />
 </p>

 - Der branch ist jetzt auf dem remote und kann auf der Gitlab Seite eingesehen werden

 - Wenn die Änderungen fertig sind kann ein Merge Request erstellt werden

 - Bei einem Merge Request wird der aktuelle branch mit dem main branch gemerged (zusammengeführt)

 - Dazu muss auf der Gitlab Seite auf "Merge Request" geklickt werden

 <p align="center">
 <img src="img/img_21.png" alt="" width="500" margin="auto" />
 </p>

 - Hier wird der aktuelle branch ausgewählt und der main branch als Ziel

 <p align="center">
 <img src="img/img_22.png" alt="" width="500" margin="auto" />
 </p>

 - Dann kann der Merge Request erstellt werden

 <p align="center">
 <img src="img/img_23.png" alt="" width="500" margin="auto" />
 </p>

 - Der Merge Request kann dann augeführt werden

 <p align="center">
 <img src="img/img_24.png" alt="" width="500" margin="auto" />
 </p>

 - Wenn man nun in GitAhead den main branch auswählt und auf "Pull" klickt werden die Änderungen vom remote heruntergeladen

 <p align="center">
 <img src="img/img_25.png" alt="" width="500" margin="auto" />
 </p>

 - Das Bild zeigt wie sich der main branch von main abspaltet, dann daran Anderungen gemacht weden und dann wieder mit main zusammengeführt wird

 <p align="center">
 <img src="img/img_26.png" alt="" width="400" margin="auto" />
 </p>

 - optional kann der branch noch gelöscht werden

 <p align="center">
 <img src="img/img_27.png" alt="" width="500" margin="auto" />
 </p>

 <p align="center">
 <img src="img/img_28.png" alt="" width="300" margin="auto" />
 </p>






