#include "gamelib.h"

static Zona *prima_zona = NULL;
static Zona *ultima_zona = NULL;

static Giocatore *giocatore[numero_giocatori];

int cntZona = 0;

//int array_ordine[numero_giocatori];
//int *pnt_array_ordine = &array_ordine[0];

//funzioni che gestiscono la Mappa
//___________________________________________________________________________________________________________________________________________

void gest_map() //funzione che gestisce il menu "crea mappa"
{
	int op_map = 0, i_zona = 0;

	do
	{
		printf("\n1_Inserisci Zone\n2_Elimina Ultima Zona\n3_Elimina Mappa\n4_Stampa Mappa\n5_Chiudi Mappa\n");
		scanf("%d", &op_map);

		switch(op_map)
		{
			case 1:
			if(flag_ins == 0)//flag_ins è una variabile per accertarsi che la mappa non sia già stata chiusa.
			{
				do
				{
					printf("\nSelezionare la zona da inserire:\n1_Cucina\n2_Soggiorno\n3_Rimessa\n4_Strada\n5_Lungo Lago\n6_FINE INSERIMENTO\n");
					scanf("%d", &i_zona);
					i_zona--;
					if(i_zona >= 0 && i_zona <= 4)
					{
						ins_zona(i_zona);
						cntZona++;
					}
					else if(i_zona == 5)
					{
						break;
					}
					else
					{
						printf("\nIl Valore non è valido!!!\n");
						i_zona = -1;
					}
				}
				while(i_zona != 5);
			}
			else
			{
				printf("\n_______!!__La mappa è già stata creata!__!!_______\n");
			}
				break;
			case 2:
				if(flag_ins == 0)
				{
					canc_zona();
				}
				else
				{
					printf("\n_______!!__La mappa è già stata creata!__!!_______\n");
				}
				break;
			case 3:
					if(prima_zona == NULL)
					{
						printf("\n_______!!__La mappa è vuota!__!!_______\n");
					}
					else
					{
						char c = ' ';
						printf("\nSei sicuro di voler continuare? S/N\n");
						scanf("%s", &c);
						do
						{
							switch (c)
							{
								case 's':
								case 'S':
									canc_mappa();
									flag_ins = 0;//in questo modo il sistema non riconosce più la mappa come già creata
									break;
								case 'n':
								case 'N':
									break;
								default:
									c = 'a';
									break;
							}
						} while(c == 'a');

					}
			break;
			case 4:
				stampa_mappa();
				break;
			case 5:
				switch (controllo_chiusura())
				{
					case 1:
						flag_ins = 1;
						cntZona++;
						ins_zona(5);
						break;
					case -1:
						printf("Non sono state create abbastanza zone.Inserisci più zone ne servono almeno 8!\n");
						op_map = 0;
						break;
					case -2:
						printf("_______!!__La mappa è vuota!__!!_______\n");
						//op_map = 5;
						break;
					case -3://questo caso evita che alla funzione vengano aggiunte zone ""uscita_campeggio" in eccesso
						//op_map = 5;
						break;
					}
						break;
				default:
					printf("Scelta Errata\n");
					break;
			}
	}
	while(op_map != 5);
}

void ins_zona(int i_zona)//funzione di inserimento zone della mappa
{
	if(prima_zona == NULL)//caso in cui la mappa è inizialmente vuota
	{
		prima_zona = (Zona *) malloc(sizeof(Zona));
		prima_zona -> numZona = cntZona;
		prima_zona -> nome_zona = i_zona;
		prima_zona -> oggetto = randomOgg(i_zona);
		prima_zona -> zona_successiva = NULL;
		ultima_zona = prima_zona;
	}
	else
	{
		ultima_zona -> zona_successiva = (Zona *) malloc(sizeof(Zona));
		ultima_zona -> numZona = cntZona;
    ultima_zona = ultima_zona -> zona_successiva;
   	ultima_zona -> nome_zona = i_zona;
  	ultima_zona -> oggetto = randomOgg(i_zona);
		ultima_zona -> zona_successiva = NULL;
	}
}

//inizializzazione funzione random
void inizializzaRand()
{
  time_t t;
  srand((unsigned) time(&t));
}

//funzione randomizzazione degli oggetti nella zona
int randomOgg(int i_zona)
{
  int randNum = (rand() % 1000);
  int count = 0;
  int i = -1;
  do
  {
    i++;
    count = count + probabilita_ogg_zona[i_zona][i];
  }
  while(count < randNum);
  return i;
}

void canc_zona()//funzione che cancella l'ultima zona creata
{
	if(prima_zona == NULL)
	{
		printf("Non ci sono zone nella mappa\n");
	}

	else if (prima_zona == ultima_zona)
	{
		free(prima_zona);
		cntZona--;
		prima_zona = NULL;
		ultima_zona = NULL;
	}

	else
	{
		Zona *temp = prima_zona;
		while(((temp -> zona_successiva) -> zona_successiva) != NULL)
		{
			temp = temp -> zona_successiva;
		}
		cntZona--;
		free(temp -> zona_successiva);
		temp -> zona_successiva = NULL;
		ultima_zona = temp;
	}
}

void canc_mappa()//funzione che cancelle l'intera mappa
{
	while (prima_zona != NULL) {
		cntZona = 0;
		flag_ins = 0;
		Zona *temp = prima_zona;
		prima_zona = prima_zona -> zona_successiva;
		free(temp);
	}
}
 void stampa_mappa()//funzione che stampa sullo schermo l'ordine delle zone
 {
	 Zona *temp = prima_zona;
	 int count_zona = 1;

	 if (prima_zona == NULL)
	 {
	 		printf("_______!!__La mappa è vuota!__!!_______\n");
	 }

	 else
	 {
		 while(temp != NULL)
		 {
			 printf("\n______________________________\n");
			 printf("%d_%s", count_zona, assoc_nome_zone [temp -> nome_zona]/*, assoc_nome_oggetti [temp -> oggetto]*/ );
			 printf("\n______________________________\n");
			 temp = temp -> zona_successiva;
			 count_zona++;
		 }
	 }
 }

int count_zona()//funzione che conta le zone presenti sulla mappa
{
	Zona *temp = prima_zona;
	int count_zona = 1;
	while(temp != NULL)
	{
		count_zona++;
		temp = temp -> zona_successiva;
	}
	return count_zona;
}

int controllo_chiusura()//funzione che controlla che la mappa possa essere chiusa correttamente
 {
	 if(prima_zona == NULL)//mappa vuota
	 {
		 return -2;
	 }
	 else if(count_zona() < 8)//caso in cui non ci sono abbastanza zone
	 {
		 return -1;
	 }
	 else if (ultima_zona -> nome_zona == 5)//caso in cui la mappa è già stata creata correttamente
	 {
	 	 return -3;
	 }
	 else
	 {
		 return 1;
	 }
 }


void iniz_zaino (Giocatore *giocatore)
{
	for (int i = 0; i < sizeof(giocatore -> Zaino)/4; i++)
	{
		giocatore -> Zaino[i] = 0;
	}
}

void inizializza_partita(int num_giocatori)
{
	for (int i = 0; i < num_giocatori; i++)
	{
		giocatore[i] = (Giocatore *)malloc(sizeof(Giocatore));
		giocatore[i] -> i_player = i;
		giocatore[i] -> stato = 0;
		giocatore[i] -> posizione = prima_zona;
		giocatore[i] -> win = 0;
		iniz_zaino(giocatore[i]);
	}
	strcpy(giocatore[0] -> nome_giocatore, "Giacomo");
	strcpy(giocatore[1] -> nome_giocatore, "Marzia");
	giocatore[0] -> Zaino[2] = 1;
	giocatore[1] -> Zaino[5] = 2;
}
	/*
	//primo Giocatore
	primo = (Giocatore*)malloc(sizeof (Giocatore));
	primo -> i_player = 0;
	strcpy(primo -> nome_giocatore, "Giacomo");
	primo -> stato = 0;
	primo -> posizione = prima_zona;
	iniz_zaino(primo);
	primo -> Zaino[2] = 1;

	//secondo Giocatore
	secondo = (Giocatore*)malloc(sizeof(Giocatore));
	secondo -> i_player = 1;
	strcpy(secondo -> nome_giocatore, "Marzia");
	secondo -> stato = 0;
	secondo -> posizione = prima_zona;
	iniz_zaino(secondo);
	secondo -> Zaino[5] = 2;
	*/


void gioca()
{
	int player = 0;
	int ins_play = 0;
	char i_exit = ' ';
	int a = 0;
	int turno = 0;
	int countFase = 0;


		//player = rand_giocatore();
		do
		{
			if (giocatore[0] -> stato != 2 && giocatore[1] -> stato != 2)
			{
				if(flagChange == 0)
				{
					if(countFase == 0)
					{
						player = rand() % numero_giocatori;
						countFase++;
					}
					else
					{
						player = switch_player(player);
						countFase = 0;
					}

					if (countFase == 1)
					{
						turno++;
					}
				}
				else
				{
					flagChange--;
				}
			}

			else if(giocatore[0] -> stato != 2 || giocatore[1] -> stato != 2)
			{
				if(flagChange == 0)
				{
					if(giocatore[0] -> stato == 2)
					{
						player = 1;
					}
					else
					{

						player = 0;
					}
					turno++;
				}
				else
				{
					flagChange--;
				}
			}

			if (giocatore[0] -> stato != 2 || giocatore[1] -> stato != 2)
			{
				printf("\nZona Attuale: %s\nZona %d di %d\n", assoc_nome_zone[giocatore[player] -> posizione -> nome_zona], giocatore[player] -> posizione -> numZona, cntZona);
				printf("Turno %d, %s (%s)\n", turno, giocatore[player] -> nome_giocatore, assoc_stato[giocatore[player] -> stato]);
				printf("\nSeleziona un'azione da compiere:\n1_Cerca Oggetto\n2_Prendi Oggetto\n3_Avanza alla Zona Successiva\n4_Cura\n5_Usa Adrenalina\n\n6_Visualizza oggetti nello zaino\n7_Esci dal Gioco\n");
				scanf("%d", &ins_play);
			}


			do{
				switch (ins_play)
				{
					case 1:
						if (giocatore[player]-> posizione -> oggetto !=-1)
						{
							printf("\nHai trovato %s\n", assoc_nome_oggetti[mostra_oggetto(giocatore[player] -> posizione)]);
							ins_play = 7;
						}
						else
						{
							printf("\nSpiacente ma non hai trovato niente di utile!\n");
						}
						ins_play = 0;
						break;
					case 2:
						if(prendi_oggetto(giocatore[player] -> posizione, giocatore[player]) == 0)
						{
							printf("\nNella Zona non ci sono oggetti\n");
							flagChange = 1;
						}
						else
						{
							prendi_oggetto(giocatore[player] -> posizione, giocatore[player]);
						}
						ins_play = 7;
						break;
					case 3:
						if((giocatore[player] -> posizione) -> nome_zona !=5)
						{
							avanza(giocatore[player]);
							printf("\nTi sei spostato a %s\n", assoc_nome_zone[giocatore[player] -> posizione -> nome_zona]);
							ins_play = 7;
						}

						else
						{
							printf("Ce l'hai quasi fatta Riuscirà %s a beffare Gieson?\n", giocatore[player] -> nome_giocatore);
							ins_play = 7;
							giocatore[player] -> win = 1;
						}

						break;
					case 4:
						switch(cura(giocatore[player]))
						{
							case 0:
								cura(giocatore[player]);
								printf("Sei ritornato in forma almeno per ora...");
								break;
							case -1:
								printf("\nNon c'è alcun bisogno di curarti sei in perfetta salute!\n");
								flagChange++;
								break;
							case -3:
								printf("\nNon hai bende, rimarrai ferito.\n");
								flagChange++;
								break;
						}
						ins_play = 7;
						break;
					case 5:
						if(usa_adrenalina(giocatore[player]) == 1)
						{
							//usa_adrenalina(giocatore[player]);
							flagChange = 2;
							printf("\nHai usato l'adrenalina. Puoi fare altre due mosse.\n");
						}
						else
						{
							printf("\nNon possiedi adrenalina.\n");
							flagChange++;
						}
						ins_play = 0;
						break;
					case 6:
						visualizza_zaino(giocatore[player]);
						flagChange++;
						ins_play = 0;
						break;
					case 7:
						printf("\nSei sicuro di voler uscire dal gioco? (S/N)\nCosì facendo perderai tutti i tuoi progressi!\n");
						scanf("%s", &i_exit);
						switch(i_exit)
						{
							case 's':
							case 'S':
								dealloca_dati();
								canc_mappa();
								ins_play = 9;
								break;
							case 'n':
							case 'N':
								ins_play = 0;
								flagChange++;
								break;
							default :
								printf("\nInserimento non valido\n");
								ins_play = 0;
								flagChange++;
								break;
						}
						break;
					default:
						printf("\nInserimento non valido\n");
						flagChange++;
						ins_play = 0;
						break;
				}
			} while(ins_play < 7 && ins_play >= 1);

				if (ins_play == 9)
				{
					break;
				}
				else
				{
					if (flagChange == 0)
					{
						if(benza == 0)
						{
							if(prob_gieson(giocatore[player], giocatore[switch_player(player)], giocatore[switch_player(player)] -> posizione) == 1)
							{
								gieson(giocatore[player]);
								if (giocatore[player] -> stato == 2)
								{
									printf("Gieson sta per avere la meglio su" "%s""Gieson sta per completare la sua vendetta...", giocatore[player] -> nome_giocatore);
								}
								else if(giocatore[player] -> win == 1)
								{
									printf("%s sei riuscito/a a sopravvivere a Gieson e tornare sano e salvo da questa avventura ,Bravo! Gliela hai fatta a Gieson\n", giocatore[player] -> nome_giocatore);
									giocatore[player] -> stato = 2;
								}
							}
						}
						else
						{
							benza--;
						}
						ins_play = 0;
					}
				}

				/*else
				{
					ins_play = 8;
				}*/

				if(giocatore[0] -> win == 1 && giocatore[1] -> win == 1)
				{
					printf("Incredibile tutti e due i ragazzi sono salvi davvero incredibile.Complimenti!\n");
					dealloca_dati();
					canc_mappa();
					ins_play = 9;
				}

				else if(giocatore[0] -> stato == 2 && giocatore[1] -> stato == 2)
				{
					printf("\n\nNonostante tutti gli sforzi e la tenacia dei nostri due ragazzi, Gieson li ha sopraffatti portando a termine la sua vendetta\n\nmi dispiace... HAI PERSO\n");
					dealloca_dati();
					canc_mappa();
					ins_play = 9;
				}


				/*if(esci() == 1)
				{
					ins_play = 9;
				}*/

	}while(ins_play != 9);
}

int mostra_oggetto(Zona * posizione_attuale)
{
	return posizione_attuale -> oggetto;
}

int prendi_oggetto(Zona * posizione_attuale, Giocatore * giocatore)
{
  if(posizione_attuale -> oggetto != -1)
	{
		giocatore -> Zaino[posizione_attuale -> oggetto]++;
		printf("Hai preso %s\n",assoc_nome_oggetti[posizione_attuale -> oggetto]);
		posizione_attuale -> oggetto = -1;
		return -1;
	}
	else
	{
		return 0;
	}
}

void visualizza_zaino(Giocatore * giocatore)
{
	printf("Zaino di %s\n", giocatore -> nome_giocatore);
		for (int i = 0; i < sizeof(giocatore -> Zaino)/4; i++)
		{
			printf("|%s x %d|\n", assoc_nome_oggetti[i], giocatore -> Zaino[i]);
		}
}

void avanza(Giocatore * giocatore)
{
	giocatore -> posizione = (giocatore -> posizione) -> zona_successiva;
}

int cura(Giocatore * giocatore)
{
	switch(giocatore -> stato)
	{
		case 0:
			return -1;
		case 1:
			if(giocatore -> Zaino[1] != 0)
			{
				giocatore -> stato = 0;
				giocatore -> Zaino[1]--;
				return 0;
			}
			else
			{
				return -3;
			}
	}
}

int usa_adrenalina(Giocatore * giocatore)
{
	if (giocatore -> Zaino[5] != 0)
	{
		giocatore -> Zaino[5]--;
		return 1;
	}
	else
	{
		return 0;
	}
}

void dealloca_dati()
{
	for (int i = 0; i < numero_giocatori; i++)
	{
		free(giocatore[i]);
		giocatore[i] = NULL;
	}
}

int switch_player(int player)
{
	switch (player)
	{
		case 0:
			player = 1;
			break;
		case 1:
			player = 0;
			break;
	}
	return player;
}

int prob_gieson(Giocatore * giocatore_1, Giocatore * giocatore_2, Zona * posizione_attuale)
{
	int p_gieson = 0;
	p_gieson = rand() %1000;
	if(posizione_attuale -> nome_zona != 5)
		{
			if(giocatore_1 -> stato != 2 && giocatore_2 -> stato != 2)
			{
				if(p_gieson <= 300)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				if(p_gieson <= 500)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}

	else
	{
		if(p_gieson <= 750)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int gieson(Giocatore * giocatore)
{
		printf("\nOh Nooo!è Gieson!!!\a\n");
		if (giocatore -> Zaino[2] != 0 || giocatore -> Zaino[3] !=0 || giocatore -> Zaino[4] != 0)
		{
			int ins_gie = 0;
			do
			{
				printf("Cosa vuoi fare per soprevvivere?\n1_Usa Coltello\n2_Usa Pistola\n3_Usa Benzina\n4_Arrenditi non hai scampo!\n");
				visualizza_zaino(giocatore);
				scanf("%d", &ins_gie);

				switch (ins_gie)
				{
					case 1:
						giocatore -> stato++;
						giocatore -> Zaino[2]--;
						printf("\nHai tirato fuori il coltello,sei riuscito a cacciare per il momento Gieson.... ma è riuscito comunque a ferirti\n");
						break;
					case 2:
						giocatore -> Zaino[3]--;
						printf("\nOttima mira!sei riuscito a sparare a Gieson\n ...scappa. Ma tornerà più infuriato di prima...\n");
						break;
					case 3:
						giocatore -> Zaino[4]--;
						printf("\nGieson odia la benzina! E ancora di più il fuoco!\nSei riuscito a dare fuoco a Gieson. Sicuramente non apparirà più per almeno 4 turni\n");
						benza = 4;
						break;
					case 4:
						giocatore -> stato = 2;
						printf("\nMi dispiace ma te la sei voluta!Muori\n");
						break;
					default:
						break;
				}
			}while(ins_gie < 1 && ins_gie > 4);
		}
		else
		{
			printf("Mi dispiace, ma non hai modo di difenderti. Gieson Ti uccide\n");
			giocatore -> stato = 2;
		}
	}
