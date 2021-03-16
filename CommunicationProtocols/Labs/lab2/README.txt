Buzatu-Pahontu Mihnea-Nicolae

lab 2 PC

Am trimis fisierul astfel:
1) In primul mesaj trimit numele fisierului
Receiverul mai adauga "new" la inceputul fisierului pe care il creaza.

2) In al doilea mesaj trimit marimea fisierului
Receiverul tine cont de aceasta pentru a determina cand a terminat de primit fisierul.

3) In urmatoarele mesaje trimit informatia din interiorul fisierului.
Folosesc o constanta BUFFER_SIZE ca sa determin cati bytes trimit o data. L-am setat mai mic
pentru a verifica corectitudinea programului.

* send primeste fisierul de trimis in linie de commanda

Exemplu rulare: 
	mihnea@ArchDesktop:schelet lab2$ ./run_experiment.sh 
	[sender] Sent file name
	[recv] Got filename: <test.txt>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	[sender] Sent file size
	[recv] Got filesize: <93>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	[sender] Sent information of size <16>
	[recv] Got information of size <16>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	[sender] Sent information of size <16>
	[recv] Got information of size <16>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	[sender] Sent information of size <16>
	[recv] Got information of size <16>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	[sender] Sent information of size <16>
	[recv] Got information of size <16>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	[sender] Sent information of size <16>
	[recv] Got information of size <16>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	[sender] Sent information of size <13>
	[recv] Got information of size <13>, sending ACK...
	[recv] ACK sent
	[sender] Got response: ACK
	mihnea@ArchDesktop:schelet lab2$ diff newtest.txt test.txt 
	mihnea@ArchDesktop:schelet lab2$
