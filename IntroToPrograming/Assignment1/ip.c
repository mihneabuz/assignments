//Buzatu-Pahontu Mihnea-Nicolae
//315CB

#include <stdio.h>

unsigned int citire(void)
{
	//citeste o adresa ip sau o masca
	unsigned int x, a[4];
	x = 0;
	scanf("%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]);
	x = x|(a[0]<<24);
	x = x|(a[1]<<16);
	x = x|(a[2]<<8);
	x = x|a[3];
	return x;
}

unsigned int mskconv(unsigned int x)
{
	//transforma o masca din forma /K in forma zecimala x.x.x.x
	unsigned int y;
	y = 0;
	return (~y)<<(32-x);
}

unsigned int retea(unsigned int x, unsigned int y)
{
	//returneaza adresa de retea
	return x & y;
}

_Bool checkmsk(unsigned int x)
{
	//verifica o masca daca este valida
	int i, ok;
	ok = 1;
	for (i=31; i>=0; i--)
	{
		if (x & (1<<i))
		{
			if (ok==0)
				return 0;
		}
		else
		{
			if (ok==1)
				ok = 0;
		}
	}
	return 1;
}

unsigned int repair(unsigned int x)
{
	//corecteaza o masca nevalida
	unsigned int  m;
	m = 0;
	int i;
	for (i=31; i>=0; i--)
	{
		if (x & (1<<i))
		{
			m = m|(1<<i);
		}
		else
		{
			return x & m;
		}
	}
	return 0;
}

unsigned int broadcast(unsigned int x, unsigned int y)
{
	//returneaza adresa de brodcast
	return x|(~y);
}

void afis(unsigned int x)
{
	//afiseaza o adresa ip sau o masca
	printf("%d.%d.%d.%d", x>>24, (x<<8)>>24, (x<<16)>>24, (x<<24)>>24);
}

void afisoctal(unsigned int x)
{
	//afiseaza o masca in octal
	printf("%o.%o.%o.%o", x>>24, (x<<8)>>24, (x<<16)>>24, (x<<24)>>24);
}

void afishexa(unsigned int x)
{
	//afiseaza o masca in hexadecimal
	printf("%X.%X.%X.%X", x>>24, (x<<8)>>24, (x<<16)>>24, (x<<24)>>24);
}

void afisbinar(unsigned int x)
{
	//afiseaza o adresa in binar
	int i;
	for (i=31; i>=0; i--)
	{
		if (x & (1<<i))
			printf("1");
		else
			printf("0");

		if (i%8==0 && i)
			printf(".");
	}
}

void endl(void)
{
	//new line :)
	printf("\n");
}

int main()
{
	unsigned int ip1, ip2, msk1, msk2[2], N, k, i, j, m, ip, msk;
	//msk2[1] este forma /K a mastii 2 (MSK_2), iar msk[0] este forma zecimala
	scanf("%d", &k);
	for (i=1; i<=k; i++)
	{
		//citire date
		msk1 = citire();
		scanf("%d", &msk2[1]);
		msk2[0] = mskconv(msk2[1]);
		ip1 = citire();
		ip2 = citire();
		scanf("%d", &N);

		printf("%d\n", i);

		//Task0
		printf("-0 ");
		afis(ip1);
		printf("/%d\n", msk2[1]);

		//Task1
		printf("-1 ");
		afis(msk2[0]);
		endl();

		//Task2
		printf("-2 ");
		afisoctal(msk1);
		printf(" ");
		afishexa(msk1);
		endl();

		//Task3
		printf("-3 ");
		afis(retea(ip1, msk2[0]));
		endl();

		//Task4
		printf("-4 ");
		afis(broadcast(ip1, msk2[0]));
		endl();

		//Task5
		printf("-5 ");
		if (retea(ip1, msk2[0])==retea(ip2, msk2[0]))
			printf("DA \n");
		else
			printf("NU \n");

		//Task6
		printf("-6 ");
		if (checkmsk(msk1))
			printf("DA \n");
		else
		{
			printf("NU \n");
			msk1 = repair(msk1);
		}

		//Task7
		printf("-7 ");
		afis(msk1);
		endl();

		//Task8
		printf("-8 ");
		afisbinar(ip1);
		endl();

		//Task9
		printf("-9 ");
		for (j=0; j<N; j++)
		{
			ip = citire();
			scanf("/%d", &m);
			msk = mskconv(m);
			if (retea(ip2, msk)==retea(ip, msk))
				printf("%d ", j);
		}
		endl();

	}
	return 0;
}
