Buzatu-Pahontu Mihnea-Nicolae
324CB

lab 5 - PC

ping

Am setat informatiile necesare in header-ele ip si icmp.
ttl l-am setat 32;
saddr l-am obtinut din get_interface_ip;
Am verificat pachetul de return daca e reply si am afisat.


traceroute

Am folosit aceeasi metoda ca la ping, dar am pornit cu un ttl 1.
La fiecare ping am incrementat ttl si am verificat daca am primit
timeout de la alta adresa. Cand primesc echo reply ma opresc.
