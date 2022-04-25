Buzatu-Pahontu Mihnea-Nicolae
324CB

Tema 2 PC - Aplicatie Client-Server

server.cpp
	Contine implementarea server-lui. Acesta dechide un socket TCP si unul UDP.
	Pe socketul UDP asteapta mesaje de forma publish_msg, iar pe cel TCP asteapta
	cereri de conectare de la client;.
		
	Structuri de date folosite:
		Serverul foloseste doua std::unordered_map
			- Primul mapeaza de la ID client la o structura de forma Client
				struct Client {
					bool online;        // status client
					int socket;         // socket folosit
					vector<shared_ptr<char*>> post_queue;  // coada de mesaje 
				}

			- Al doilea mapeaza de la Topic la std::vector de clienti abonati

		Aceste structuri de date permit gestionarea rapida a mesajelor.
		Majoritatea Request-urilor clientilor se executa in timp constant O(1),
		cu exceptia UNSUBSCRIBE care implica stergerea dintr-un std::vector.
		Trimiterea postarilor se executa in timp liniar O(n), unde n este
		numarul de abonati la topicul respectiv.

	Logica de functionare:
		- Serverul foloseste select pentru multiplexarea IO

		- Daca primeste un mesaj pe socketul TCP, este o cerere de conexiune
			=> atribuie noului client un socket si il adauga la fd_set

		- Daca primeste un mesaj pe socketul UDP, este o postare ce trebuie trimisa
		  	=> gaseste abonatii la topic din map, si trimite la fiecare postarea:
				* client online, indiferent SF => trimite direct
				* client offline, dar SF setat => adauga in coada clientului
				* cleint offline, SF nesetat   => ignora clientul si trece mai departe

		- Daca primeste un mesaj pe alt socket, este un client_request
			
			Protocol comunicare Client->Server
			struct client_request {
				char ID[10];              // ID of client
				char type;                // Type of request
				char topic[50];           // Topic requested
				char flags;               // Additional flags
			} __attribute__((packed));

			- daca este IDENTIFY, adauga clientul, il seteaza online si trimite
			  mesajele pe care le-a pierdut cand era offline (daca are SF)

			- daca este SUBSCRIBE, adauga clientul la lista clientilor abonati

			- daca este UNSUBSCRIBE, scoate clientul din lista clientilor abonati

			- daca este DISCONNECT, seteaza clientul ca offline si inchide socketul

		Obs. Pentru trimiterea postarilo clientilor, serverul parseaza informatia si
		genereaza string-ul corespunzator, dupa care il trimite direct prin TCP.
		Astfel nu este nevoie de un protocol pentru conexiunea Server->Client, deoarece
		clientul poate afisa toate caracterele pe care le citeste de pe socket.

subscriber.cpp
	Contine implementarea client-ului.
	Acesta comunica cu serverul folosind protocolul explicat mai sus.
		- Cand primeste o comanda de la stdin, o parseaza si trimite mesajul
		  corespunzator serverului.
		- Cand primeste un mesaj TCP de la server, afiseaza continutul acestuia ca
		  un string.
		- Daca mesajul primit contine sirul "DISCONNECT_SIGNAL", inseamna ca serverul
		  a trimis semnalul de oprire, iar clientul se inchide.

structs.h/structs.cpp:
	Contine definitiile structurilor folosite si cateva functii ajutatoare.

La rularea manuala, merg toate functionalitatile server-ului/client-ului.
Tema trece toate testele pe checkerul rulat local.
Uneori checkerul local iese cu eroarea BrokenPipeError, dar de obicei functioneaza 
la urmatoarea rulare.
De asemenea, daca este rulat fara sudo, pica testul quick_flow cu eroarea:
Error: not enough permissions to write to procfs

	- Ouput sudo python3 test.py
Compiling
g++    -c -o structs.o structs.cpp
g++ -Wall -Wextra server.cpp structs.o -o server
g++ -Wall -Wextra subscriber.cpp structs.o -o subscriber
Starting the server
Starting subscriber C1
Generating one message for each topic
Subscribing C1 to all topics without SF
Generating one message for each topic
Disconnecting subscriber C1
Generating one message for each topic
Starting subscriber C1
Starting another subscriber with ID C1
Starting subscriber C2
Subscribing C2 to topic a_non_negative_int without SF
Subscribing C2 to topic a_negative_int with SF                                                 
Generating one message for topic a_non_negative_int                                            
Generating one message for topic a_negative_int                                                
Disconnecting subscriber C2                                                                    
Generating one message for topic a_non_negative_int                                            
Generating three messages for topic a_negative_int                                             
Starting subscriber C2                                                                         
Generating one message for each topic 30 times in a row                                        
Stopping the server                                                                            
rm -f server subscriber structs.o                                                              
                                                                                               
RESULTS                                                                                        
-------                                                                                        
compile...........................passed                                                       
server_start......................passed                                                       
c1_start..........................passed                                                       
data_unsubscribed.................passed                                                       
c1_subscribe_all..................passed                                                       
data_subscribed...................passed                                                       
c1_stop...........................passed
c1_restart........................passed
data_no_clients...................passed
same_id...........................passed
c2_start..........................passed
c2_subscribe......................passed
c2_subscribe_sf...................passed
data_no_sf........................passed
data_sf...........................passed
c2_stop...........................passed
data_no_sf_2......................passed
data_sf_2.........................passed
c2_restart_sf.....................passed
quick_flow........................passed
server_stop.......................passed
