#include <SDL.h>
#include <time.h>
#include <math.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define GRANDE 1
#define MEDIO 2
#define PEQUENO 3
#define RAIO_JOGADOR 50
#define RAIO_GRANDE 50
#define RAIO_MEDIO 35
#define RAIO_PEQUENO 25
#define RAIO_TIRO 10
#define NAO 0
#define SIM 1


/*variaveis de imagens...
3 para os diferentes tamanhos de inimigos
1 para o jogador
1 para o tiro*/
SDL_Surface* inimigoGrande;
SDL_Surface* inimigoMedio;
SDL_Surface* inimigoPequeno;
SDL_Surface* jogador;
SDL_Surface* tiro;

extern SDL_Surface* screen;
SDL_Surface* carimbo;
SDL_Rect quadro;

extern TTF_Font *fonte;
extern void desenhaTexto(char *str, int x, int y, int r, int g, int b, TTF_Font *fonte, int alpha, Uint8 alphaValor,SDL_Surface * tela);

Mix_Chunk *somPerdeu,*somTiro;

unsigned long int score = 0, tempoFerido = 0;
int ferido = SIM, vida;
char temp[30];


/* Estrutura para os inimigos*/
typedef struct _inimigo
{
     int tam; //tentar usar sem colocar o raio dentro da estrutura 
     float x,y,velx,vely;
}INIMIGO;


/* Estrutura para os tiros*/
typedef struct _tiro
{
     float x,y,velx,vely;
     int grudado, forte;  //1 ou 0, sim ou nao, diz se está grudado no jogador e se está com força...
}TIRO;


// Ponteiros para os inimigos e para os tiros
INIMIGO *inimigos;
TIRO *tiros;



INIMIGO* bootInimigos (int *quantidadeInimigos) //faz o load a cada nova cena, e altera a quantidade de inimigos...
{
     int i;
     *quantidadeInimigos = 3;// quantidade de inimigos iniciais no novo cenario
     inimigos = (INIMIGO*) realloc (inimigos,*quantidadeInimigos * sizeof(INIMIGO)); //deixa 3 inimigos grandes para o cenario novo...
     
     for (i=0; i<(*quantidadeInimigos); i++)
     {
          (inimigos+i)->tam = GRANDE;
          (inimigos+i)->x = rand() % (SCREEN_W - (2*RAIO_GRANDE));
          (inimigos+i)->y = rand() % (SCREEN_W - (2*RAIO_GRANDE));
          (inimigos+i)->velx = 5 - (rand() % 10);
          (inimigos+i)->vely = 5 - (rand() % 10);
     }
     return inimigos;
}

void testaFerido() //testa se o jogador deve sair do modo invulneravel, no qual se encontra após ser atingido...
{
     if(tempoFerido)
     {
          if(SDL_GetTicks() > (tempoFerido + 1500))
          {
               ferido = NAO;
               tempoFerido = 0;
          }
     }
}

void bootCenario (int nivel) //faz o load a cada nova cena...
{
     //carrega os audios
     somTiro = Mix_LoadWAV("somTiro.wav");
     somPerdeu = Mix_LoadWAV("somPerdeu.wav");
     
   //carrega as imagens
     switch (nivel) // poderia ser feito apenas somando strings de acordo com o nível,mas fiquei com preguiça de fazer
     {
          case 1:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo1g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo1m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo1p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador1.bmp");
               tiro = SDL_LoadBMP("imagens/tiro1.bmp");
               break;
          case 2:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo2g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo2m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo2p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador2.bmp");
               tiro = SDL_LoadBMP("imagens/tiro2.bmp");
               break;
          case 3:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo3g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo3m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo3p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador3.bmp");
               tiro = SDL_LoadBMP("imagens/tiro3.bmp");
               break;
          case 4:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo4g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo4m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo4p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador4.bmp");
               tiro = SDL_LoadBMP("imagens/tiro4.bmp");
               break;
          case 5:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo5g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo5m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo5p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador5.bmp");
               tiro = SDL_LoadBMP("imagens/tiro5.bmp");
               break;
          case 6:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo6g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo6m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo6p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador6.bmp");
               tiro = SDL_LoadBMP("imagens/tiro6.bmp");
               break;
          case 7:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo7g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo7m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo7p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador7.bmp");
               tiro = SDL_LoadBMP("imagens/tiro7.bmp");
               break;
          case 8:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo8g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo8m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo8p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador8.bmp");
               tiro = SDL_LoadBMP("imagens/tiro8.bmp");
               break;
          case 9:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo9g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo9m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo9p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador9.bmp");
               tiro = SDL_LoadBMP("imagens/tiro9.bmp");
               break;
          case 10:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo10g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo10m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo10p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador10.bmp");
               tiro = SDL_LoadBMP("imagens/tiro10.bmp");
               break;
          case 11:
               inimigoGrande = SDL_LoadBMP("imagens/inimigo11g.bmp");
               inimigoMedio = SDL_LoadBMP("imagens/inimigo11m.bmp");
               inimigoPequeno = SDL_LoadBMP("imagens/inimigo11p.bmp");
               jogador = SDL_LoadBMP("imagens/jogador11.bmp");
               tiro = SDL_LoadBMP("imagens/tiro11.bmp");
               break;
     }
     
     if (inimigoGrande == NULL) puts("Não foi possível carregar inimigoGrande...");
     if (inimigoMedio == NULL) puts("Não foi possível carregar inimigoMedio...");
     if (inimigoPequeno == NULL) puts("Não foi possível carregar inimigoPequeno...");
     if (jogador == NULL) puts("Não foi possível carregar jogador...");
     if (tiro == NULL) puts("Não foi possível carregar tiro...");
     
}

TIRO* bootTiros (int *quantidadeTiros) //faz o load a cada nova cena, e altera a quantidade de tiros...
{
    int i;
     *quantidadeTiros = 1;
     tiros = (TIRO*) realloc (tiros,*quantidadeTiros * sizeof(TIRO));
     
     for (i=0; i<(*quantidadeTiros); i++)
     {
         tiros->grudado = NAO;
         tiros->forte = NAO;
         tiros->x = rand() % (SCREEN_W - (2*RAIO_TIRO));// sorteia de forma que esteja inteira na tela
         tiros->y = rand() % (SCREEN_H - (2*RAIO_TIRO));
         tiros->velx = 5 - (rand() % 10); // sorteia entre -5 e +5
         tiros->vely = 5 - (rand() % 10);
         
     }
     return tiros;
}

void confereGrude(TIRO* tiroTeste, int jogadorx, int jogadory) //verifica se algum tiro grudou no jogador...
{
     //float distancia = sqrt(pow(((tiroTeste->x + RAIO_TIRO) - (jogadorx+RAIO_JOGADOR)),2) + pow(((tiroTeste->y + RAIO_TIRO) - (jogadory+RAIO_JOGADOR)),2));
     float distancia = sqrtf( pow((jogadorx+RAIO_JOGADOR)-(tiroTeste->x+RAIO_TIRO),2) + pow((jogadory+RAIO_JOGADOR)-(tiroTeste->y+RAIO_TIRO),2) );
     
     if (distancia < (RAIO_JOGADOR + RAIO_TIRO))
     {
          tiroTeste->grudado = SIM;
          //calcular seno e coseno
          float cos = ((tiroTeste->x + RAIO_TIRO) - (jogadorx+RAIO_JOGADOR)) / distancia;
          float sen = ((tiroTeste->y + RAIO_TIRO) - (jogadory+RAIO_JOGADOR)) / distancia;
          
          //posiçaox do tiro sera o cosseno * distancia, tendo o jogadorx como origem
          tiroTeste->x = (jogadorx+RAIO_JOGADOR) + ((RAIO_JOGADOR + RAIO_TIRO) * cos);
          tiroTeste->y = (jogadory+RAIO_JOGADOR) + ((RAIO_JOGADOR + RAIO_TIRO) * sen);
     }
     /*
     if (distancia < (RAIO_JOGADOR + RAIO_TIRO))
     {
          tiroTeste->grudado = 1;
          float catetoa, catetob;
          
          while (distancia < (RAIO_JOGADOR + RAIO_TIRO))
          {
             catetoa = (tiroTeste->y +RAIO_TIRO) - (jogadory + RAIO_JOGADOR);
             catetob = (tiroTeste->x +RAIO_TIRO) - (jogadorx + RAIO_JOGADOR);
             float sen = catetoa/distancia;
             float cos = catetob/distancia;
             
             if (sen < 0) tiroTeste->y -= 1;
             if (sen > 0) tiroTeste->y += 1;
             if (cos < 0) tiroTeste->x -= 1;
             if (cos > 0) tiroTeste->x += 1;
             distancia = sqrtf(pow(((tiroTeste->x + RAIO_TIRO) - (jogadorx + RAIO_JOGADOR)),2) + pow(((tiroTeste->y + RAIO_TIRO) - (jogadory + RAIO_JOGADOR)),2));
          }
     }*/
}

void quebraInimigo(INIMIGO* inimigoAtingido, TIRO* Tiro, int* quantidadeInimigos, int* quantidadeTiros)
{
     int raioAntes, raioDepois; // raio antes e depois de ser atingido
     
     if (inimigoAtingido->tam == PEQUENO) // se ele for pequeno deve ser eliminado e deve-se criar um tiro
     {
          // /remove o inimigo
          *(inimigoAtingido) = *(inimigos+(*quantidadeInimigos)-1); // o conteudo do q será¡ removido recebe o conteudo do ultimo.
          (*quantidadeInimigos) = (*quantidadeInimigos)-1;
          inimigos = realloc (inimigos,(*quantidadeInimigos) * sizeof(INIMIGO));
          // /terminou de remover
          
          Tiro->velx = (Tiro->velx + Tiro->vely)/2; //gira 45 graus em relação ao tiro
          Tiro->vely = (Tiro->vely - Tiro->velx)/2;
          
          (*quantidadeTiros)=(*quantidadeTiros)+1;
          tiros = (TIRO*) realloc (tiros, *quantidadeTiros * sizeof(TIRO));
          (tiros + (*quantidadeTiros) - 1)->x = Tiro->x;
          (tiros + (*quantidadeTiros) - 1)->y = Tiro->y;
          (tiros + (*quantidadeTiros) - 1)->velx = -(Tiro->vely); // gira -90 graus em relação ao outro
          (tiros + (*quantidadeTiros) - 1)->vely = Tiro->velx;
          (tiros + (*quantidadeTiros) - 1)->forte = NAO;
          (tiros + (*quantidadeTiros) - 1)->grudado = NAO;
          
     }else // se for grande ou medio, deve mudar de tamanho e adicionar um novo tiro
     {
          switch ((inimigoAtingido)->tam) // avalia o raio do inimigo...
          {
               case 1:
                    raioAntes = RAIO_GRANDE;
                    raioDepois = RAIO_MEDIO;
                    break;
               case 2:
                    raioAntes = RAIO_MEDIO;
                    raioDepois = RAIO_PEQUENO;
                    break;
          }
          
          inimigoAtingido->tam = (inimigoAtingido->tam)+1;
          inimigoAtingido->x = inimigoAtingido->x + raioAntes - raioDepois; // faz o novo ser concentrico com o velho...
          inimigoAtingido->y = inimigoAtingido->y + raioAntes - raioDepois; // faz o novo ser concentrico com o velho...
          inimigoAtingido->velx = (Tiro->velx + Tiro->vely)/2; //gira 45 graus em relação ao tiro
          inimigoAtingido->vely = (Tiro->vely - Tiro->velx)/2;
          
          (*quantidadeInimigos) = (*quantidadeInimigos)+1;
          inimigos = (INIMIGO*) realloc (inimigos, (*quantidadeInimigos) * sizeof(INIMIGO));
          (inimigos + (*quantidadeInimigos) - 1)->tam = inimigoAtingido->tam;
          (inimigos + (*quantidadeInimigos) - 1)->x = inimigoAtingido->x;
          (inimigos + (*quantidadeInimigos) - 1)->y = inimigoAtingido->y;
          (inimigos + (*quantidadeInimigos) - 1)->velx = -(inimigoAtingido->vely); // gira -90 graus em relação ao atingido
          (inimigos + (*quantidadeInimigos) - 1)->vely = inimigoAtingido->velx;
          
          Tiro->velx = -(Tiro->velx);
          Tiro->vely = -(Tiro->vely);
          Tiro->forte = NAO;
     }
     
     
}

void confereAtaque(TIRO* tiroTeste, int* quantidadeInimigos, int* quantidadeTiros)
{
     int distx, disty, dist, raioInimigo, i;
     
     for (i=0; i< (*quantidadeInimigos); i++)
     {
          switch ((inimigos+i)->tam) // avalia o raio do inimigo...
          {
               case 1:
                    raioInimigo = RAIO_GRANDE;
                    break;
               case 2:
                    raioInimigo = RAIO_MEDIO;
                    break;
               case 3:
                    raioInimigo = RAIO_PEQUENO;
                    break;
          }
          
          distx = fabs((tiroTeste->x + RAIO_TIRO) - ((inimigos+i)->x + raioInimigo));
          disty = fabs((tiroTeste->y + RAIO_TIRO) - ((inimigos+i)->y + raioInimigo));
          dist = distx*distx;
          dist += disty*disty;
          dist = sqrt(dist);
                    
          if (dist < (RAIO_TIRO + raioInimigo)) // atingiu
          {
               quebraInimigo((inimigos+i), tiroTeste, quantidadeInimigos, quantidadeTiros);
               score += 10;
               tiroTeste->forte = NAO; // se bater em alguem ele fica fraco
          }
     }
}


void movimentaInimigos (int quantidadeInimigos, float jogadorx, float jogadory)// conferir a colisao inimigo-jogador
{    
     int raio,i,distancia; // variavel para a distancia entre o inimigo e o jogador...
     for (i=0; i<quantidadeInimigos; i++)
     {
          switch ((inimigos+i)->tam) // avalia o raio e a imagem do inimigo...
          {
               case 1:
                    carimbo = inimigoGrande;
                    raio = RAIO_GRANDE;
                    break;
               case 2:
                    carimbo = inimigoMedio;
                    raio = RAIO_MEDIO;
                    break;
               case 3:
                    carimbo = inimigoPequeno;
                    raio = RAIO_PEQUENO;
                    break;
          }
          // Atualiza a posição do inimigo...
          (inimigos+i)->x = ((inimigos+i)->x) + ((inimigos+i)->velx);
          (inimigos+i)->y = ((inimigos+i)->y) + ((inimigos+i)->vely);
          
          if ((inimigos+i)->x > SCREEN_W)
          {
               (inimigos+i)->x = -(2*raio);
          }
          else if ((inimigos+i)->x < -(2*raio))
          {
               (inimigos+i)->x = SCREEN_W;
          }
          
          if ((inimigos+i)->y > SCREEN_H)
          {
               (inimigos+i)->y = -(2*raio);
          }
          else if ((inimigos+i)->y < -(2*raio))
          {
               (inimigos+i)->y = SCREEN_H;
          }
          
          //hora de testar colisoes
          if (ferido == NAO)
          {
               distancia = sqrt(pow((((inimigos+i)->x + raio) - (jogadorx + RAIO_JOGADOR)),2) + pow((((inimigos+i)->y + raio) - (jogadory + RAIO_JOGADOR)),2));
               if (distancia < (RAIO_JOGADOR + raio))
               {
                    ferido = SIM;
                    tempoFerido = SDL_GetTicks();
                    vida--;
                    puts("Você tomou um teco...");
               }
          }
          /////////
          
          //hora de colar na tela 
          SDL_SetColorKey(carimbo, SDL_SRCCOLORKEY, SDL_MapRGB(carimbo->format, 128, 128, 128));
          
          quadro.x = (inimigos+i)->x;
          quadro.y = (inimigos+i)->y;
          quadro.w = (2*raio);
          quadro.h = (2*raio);
          SDL_BlitSurface(carimbo, NULL, screen, &quadro);
     }
}

void movimentaTiros (int* quantidadeTiros, int* quantidadeInimigos, float jogadorx, float jogadory, int* dx, int* dy)// conferir colisao tiro-inimigo tiro-jogador
{
     int i,j;
     for (i=0; i< (*quantidadeTiros); i++)
     {
          
          if ((tiros+i)->grudado == NAO) // se nao estiver grudado vai andar de acordo com sua velocidade...
          {
               j=1;
               if ((tiros+i)->forte) j=3;
               while (j)                // se estiver forte, anda tres vezes...
               {
                    j--;
                    // Atualiza a posição do tiro...
                    (tiros+i)->x = ((tiros+i)->x) + ((tiros+i)->velx);
                    (tiros+i)->y = ((tiros+i)->y) + ((tiros+i)->vely);
                    
                    if ((tiros+i)->x > SCREEN_W)
                    {
                         (tiros+i)->x = -(2*RAIO_TIRO);
                         (tiros+i)->forte = NAO;
                    }
                    else if ((tiros+i)->x < -(2*RAIO_TIRO))
                    {
                         (tiros+i)->x = SCREEN_W;
                         (tiros+i)->forte = NAO;
                    }
                    
                    if ((tiros+i)->y > SCREEN_H)
                    {
                         (tiros+i)->y = -(2*RAIO_TIRO);
                         (tiros+i)->forte = NAO;
                    }
                    else if ((tiros+i)->y < -(2*RAIO_TIRO))
                    {
                         (tiros+i)->y = SCREEN_H;
                         (tiros+i)->forte = NAO;
                    }
                    
                    //hora de testar colisoes
                    confereGrude((tiros+i), jogadorx, jogadory); // testa se grudou no jogador...
                    
                    if ((tiros+i)->forte == SIM) //se o tiro estiver no modo de ataque
                    {
                         confereAtaque((tiros+i), quantidadeInimigos, quantidadeTiros); //ve se o tiro bateu em um inimigo
                    }
               }
          }else //se estiver grudado andara de acordo com o movimento do mouse
          {
               if (*dx)
               {
                    (tiros+i)->x = ((tiros+i)->x) + (*dx);
               }
               if (*dy)
               {
                    (tiros+i)->y = ((tiros+i)->y) + (*dy);
               }
          }
          
          //hora de colar na tela 
          carimbo = tiro;
          SDL_SetColorKey(carimbo, SDL_SRCCOLORKEY, SDL_MapRGB(carimbo->format, 128, 128, 128));
          
          quadro.x = (tiros+i)->x;
          quadro.y = (tiros+i)->y;
          quadro.w = (2*RAIO_TIRO);
          quadro.h = (2*RAIO_TIRO);
          SDL_BlitSurface(carimbo, NULL, screen, &quadro);
     }
     *dx = 0;
     *dy = 0;
}

void desenhaJogador(int jogadorx, int jogadory)
{
     carimbo = jogador;
     SDL_SetColorKey(carimbo, SDL_SRCCOLORKEY, SDL_MapRGB(carimbo->format, 128, 128, 128));
     quadro.x = jogadorx;
     quadro.y = jogadory;
     quadro.w = 2*RAIO_JOGADOR;
     quadro.h = 2*RAIO_JOGADOR;
     SDL_BlitSurface(carimbo, NULL, screen, &quadro);
}



void atirar(int quantidadeTiros, int jogadorx, int jogadory)
{
     Mix_PlayChannel(-1, somTiro, 0);

     int i;
     float distancia;// = RAIO_JOGADOR;
     float catetoa, catetob, sen, cos;
     
     for (i=0; i<quantidadeTiros; i++)
     {
          //sen2 +cos2 = 1
          if ((tiros+i)->grudado == SIM)
          {
               catetoa = ((tiros+i)->y +RAIO_TIRO) - (jogadory);
               catetob = ((tiros+i)->x +RAIO_TIRO) - (jogadorx);
               distancia = sqrt(pow(catetoa,2) + pow(catetob,2));
               
               sen = catetoa/distancia;
               cos = catetob/distancia;
               
               (tiros+i)->velx = sqrt(fabs(4 - pow(sen,2))); //velocidade total será 4
               (tiros+i)->vely = sqrt(fabs(4 - pow(cos,2)));
               
               if (((tiros+i)->x + RAIO_TIRO) < (jogadorx +RAIO_JOGADOR))
               {
                    (tiros+i)->velx =-((tiros+i)->velx); // analisa em que quadrante estah o tiro...
               }
               if (((tiros+i)->y + RAIO_TIRO) < (jogadory +RAIO_JOGADOR))
               {
                    (tiros+i)->vely = -((tiros+i)->vely);
               }
               
               (tiros+i)->grudado = NAO;
               (tiros+i)->forte = SIM;
          }
     }
}





unsigned long int jogoPizza ()
{
     SDL_Event event;
     int nivel=1, sair=0, pausa=0, quantidadeInimigos=0, quantidadeTiros=0;
     int dx=0, dy=0; // deslocamento segundo o mouse...
     int jogadorx, jogadory; // posição do jogador...
     srand(time(NULL));
     vida = 6;
     tempoFerido = SDL_GetTicks();
     
     // inicia os ponteiros para inimigos e tiros
     inimigos = (INIMIGO *) malloc (1 * sizeof(INIMIGO));
     tiros = (TIRO *) malloc (1 * sizeof(TIRO));
     
     //arruma o video.
     
     inimigos = bootInimigos(&quantidadeInimigos);
     tiros = bootTiros(&quantidadeTiros);
     bootCenario(nivel);
     
     //quebraInimigo((inimigos), (tiros), &quantidadeInimigos, &quantidadeTiros);
     
     
     while(!sair)
     {
          while(SDL_PollEvent(&event))
          {
               if(event.type == SDL_QUIT)
               {
                    sair = 1;
               }else if(event.type == SDL_KEYDOWN)
               {
                    if(event.key.keysym.sym == SDLK_ESCAPE) //fecha o jogo
                    {
                         sair = 1;
                    }
                    else if(event.key.keysym.sym == SDLK_p) //pausa
                    {
                         pausa = (pausa +1) % 2;
                    }else if(event.key.keysym.sym == SDLK_n)// move para o proximo nivel...
                    {
                         nivel++;
                         inimigos = bootInimigos(&quantidadeInimigos);
                         tiros = bootTiros(&quantidadeTiros);
                         bootCenario(nivel);
                    }
               }else if (event.type == SDL_MOUSEMOTION)
               {
                    dx += (event.motion.x - RAIO_JOGADOR) - jogadorx;
                    dy += (event.motion.y - RAIO_JOGADOR) - jogadory;
                    jogadorx = event.motion.x - RAIO_JOGADOR;
                    jogadory = event.motion.y - RAIO_JOGADOR;
               }else if (event.type == SDL_MOUSEBUTTONDOWN)
               {
                    atirar(quantidadeTiros, jogadorx, jogadory);
               }
          }
          
          static unsigned long int tempo = 0;
          testaFerido();
          if (SDL_GetTicks() > tempo)
          {
               if (!pausa)
               {
                    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 50)); //desenha o fundo...
                    movimentaInimigos(quantidadeInimigos, jogadorx, jogadory);
                    desenhaJogador(jogadorx, jogadory);
                    movimentaTiros(&quantidadeTiros, &quantidadeInimigos, jogadorx, jogadory, &dx, &dy);
                    sprintf (temp,"Vidas:%d   Score:%ld",vida, score);
                    desenhaTexto(temp,20,20,255,255,255,fonte,0,0, screen);
                    SDL_Flip(screen);
                    
                    //printf("%d\n",quantidadeInimigos);
                    if (!quantidadeInimigos)
                    {
                         nivel++;
                         inimigos = bootInimigos(&quantidadeInimigos);
                         tiros = bootTiros(&quantidadeTiros);
                         bootCenario(nivel);
     
                    }
               }
               if (!vida) sair=1;
               tempo = SDL_GetTicks() + 20;
          }
     }
     Mix_PlayChannel(-1, somPerdeu, 0);
     return score;
}
