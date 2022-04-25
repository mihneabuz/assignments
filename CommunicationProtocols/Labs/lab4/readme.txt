Buzatu-Pahontu Mihnea-Nicolae 
324CB

lab 4 - PC

/TODO/ 1:
	get_best_route 
	Am parcurs toate entry-urile din table si am verificat daca ip & mask coincid
	Ca sa obtin longest prefix match, retin mask-ul entryului si caut solutia cu 
	mask-ul cel mai mare

/TODO/ 2:
	get_arp_entry
	Am parcurs entry-urile din arp_table si am returnat cand am gasit ip-ul corect

/TODO/ 3:
	checksum
	Am extras checksum din ip_hdr si il calculez local cu functia pusa la dispozitie
	Daca acestea difera, pachetul este aruncat

/TODO/ 4:
	check TTL
	Verific daca TTL >= 1, altfel pachetul este aruncat

/TODO/ 5:
	find best route
	Am folosit functia de la 1 ca sa obtin entry-ul best_route. Daca obtin NULL,
	adica nu am gasit niciun route,	pachetul este aruncat
	
/TODO/ 6:
	update ip_hdr
	Am decrementat ttl-ul si am updatat checksum-ul

/TODO/ 7:
	find arp_entry
	Am folosit functia de la 2 ca sa obtin adresa MAC a next_hop (din best_route)
	Daca obtine entry NULL, pachetul este aruncat
	Am actualizat ether_header astfel:
		ether_dhost = mac-ul obtinut din arp_table
		ether_shost = mac-ul interfetei obtinute din route table

/TODO/ 8:
	forward packet
	Am retrimis pachetul spre next_hop
