#include <SDL.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


#define SCREEN_W  800
#define SCREEN_H  600
#define TAM_RECORDE 5
#define BOTAO_X  300
#define BOTAO_Y  275
#define BOTAOESP  60 //espacamento entre os botoes.

typedef struct BOTAO_
{
     SDL_Surface * imagem;
     SDL_Rect pos, sprite;
     int emFoco;
} Botao;

typedef struct IMAGEM_
{
     SDL_Surface * imagem;
     SDL_Rect pos;
} Imagem;

typedef struct PONTOS_
{
     int multp;
     unsigned long int qtd;
     char str[30];
} Pontos;

void inicia();
void carregaMenu();
void carregaBotao(Botao * bt, char * arq, int x, int y, int qtdFrames);
void carregaImagem(Imagem * imagem, char * arq, int x, int y);
unsigned long int * carregaRecordes();
void desenhaRecordes(unsigned long int * v);
void scoreTotal(unsigned long int score);
void desenhaTexto(char *str, int x, int y, int r, int g, int b, TTF_Font *fonte, int alpha, Uint8 alphaValor, SDL_Surface * tela);
void updateMenu(int inMenu);
void adicionaRecorde(unsigned long int novaPontuacao);
void telaApresentacao(char str1[],char str2[]);
int mouseOver(Botao * bt, int x, int y);
unsigned long int jogoPizza ();

SDL_Event event;
Imagem fundoMenu, cursor;
Botao btNovo, btCreditos, btSair, btHighScores, btVoltar;
SDL_Surface *screen;
Mix_Music *musicaMenu;
Mix_Chunk *cliqueBotao, *somBebe;
TTF_Font *fonte;


int main(int argc, char * argv[])
{
     int sair = 0, inMenu = 1;
     
     
     inicia();
     carregaMenu();
     telaApresentacao("imagens/menu/apresentacao.bmp","imagens/menu/fundoMenu.bmp");
     Mix_PlayChannel(-1, somBebe, 0);
     do
     {
          updateMenu(inMenu);
          while(SDL_PollEvent(&event))
          {
               if(event.type == SDL_QUIT)
               {
                    sair = 1;
               }
               if (event.type == SDL_MOUSEMOTION)
               {
                    if (inMenu)
                    {
                         mouseOver(&btNovo, event.motion.x, event.motion.y);
                         mouseOver(&btCreditos, event.motion.x, event.motion.y);
                         mouseOver(&btHighScores, event.motion.x, event.motion.y);
                         mouseOver(&btSair, event.motion.x, event.motion.y);
                    } else
                         {
                              mouseOver(&btVoltar, event.motion.x, event.motion.y);
                         }
                    cursor.pos.x = event.motion.x;
                    cursor.pos.y = event.motion.y;
               }
               if (event.type == SDL_MOUSEBUTTONDOWN)
               {
                    if (btNovo.emFoco)
                    {    btNovo.emFoco = 0;
                         btNovo.sprite.x = 0;
                         Mix_PlayChannel(-1, cliqueBotao, 0);
                         Mix_HaltMusic();
                         scoreTotal(jogoPizza());
                         Mix_PlayMusic(musicaMenu,-1);
                    }
                    if (btCreditos.emFoco)
                    {
                         carregaImagem(&fundoMenu,"imagens/menu/creditos.bmp", 0, 0);
                         inMenu = 0;
                         btCreditos.sprite.x = 0;
                         Mix_PlayChannel(-1, cliqueBotao, 0);
                         btCreditos.emFoco = 0;
                    }
                    if (btHighScores.emFoco)
                    {
                         carregaImagem(&fundoMenu,"imagens/menu/highScores.bmp", 0, 0);
                         desenhaRecordes(carregaRecordes());
                         inMenu = 0;
                         btHighScores.sprite.x = 0;
                         Mix_PlayChannel(-1, cliqueBotao, 0);
                         btHighScores.emFoco = 0;
                    }
                    if (btVoltar.emFoco)
                    {
                         carregaImagem(&fundoMenu,"imagens/menu/fundoMenu.bmp", 0, 0);
                         inMenu = 1;
                         btVoltar.sprite.x = 0;
                         Mix_PlayChannel(-1, cliqueBotao, 0);
                         btVoltar.emFoco = 0;
                    }
                    if (btSair.emFoco)
                    {
                         sair = 1;
                    }
               }
          }
     }while(!sair);
     return 1;
}

void inicia()
{
     SDL_Init(SDL_INIT_VIDEO);
     TTF_Init();
     screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_ASYNCBLIT);
     Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512);
     SDL_WM_SetCaption("Desenvolvendo Menu","Menu!");
     //SDL_WM_GrabInput(!SDL_WM_GrabInput(SDL_GRAB_QUERY));
     SDL_ShowCursor(1);
}

void carregaMenu()
{
     carregaImagem(&fundoMenu,"imagens/menu/apresentacao.bmp", 0, 0);
     carregaImagem(&cursor,"imagens/menu/cursor.bmp", 0, 0);
     carregaBotao(&btNovo, "imagens/menu/btNovo.bmp", BOTAO_X, BOTAO_Y, 2);
     carregaBotao(&btCreditos, "imagens/menu/btCreditos.bmp", BOTAO_X, BOTAO_Y+BOTAOESP, 2);
     carregaBotao(&btHighScores, "imagens/menu/btHighScores.bmp", BOTAO_X, BOTAO_Y+(BOTAOESP*2), 2);
     carregaBotao(&btSair, "imagens/menu/btSair.bmp", BOTAO_X, BOTAO_Y+(BOTAOESP*3), 2);
     carregaBotao(&btVoltar, "imagens/menu/btVoltar.bmp", BOTAO_X, BOTAO_Y+(BOTAOESP*4), 2);
     musicaMenu = Mix_LoadMUS("audios/menu/musicaMenu.mp3");
     cliqueBotao = Mix_LoadWAV("audios/menu/cliqueBotao.wav");
     somBebe = Mix_LoadWAV("audios/menu/somBebe.wav");
     Mix_PlayMusic(musicaMenu,-1);
     fonte = TTF_OpenFont("Kingthings Trypewriter.ttf", 30);
}

void updateMenu(int inMenu)
{
     SDL_FillRect(screen, 0, 0);
     SDL_BlitSurface(fundoMenu.imagem, 0, screen, &fundoMenu.pos);
     if (inMenu)
     {
          SDL_BlitSurface(btNovo.imagem, &btNovo.sprite, screen, &btNovo.pos);
          SDL_BlitSurface(btCreditos.imagem, &btCreditos.sprite, screen, &btCreditos.pos);
          SDL_BlitSurface(btHighScores.imagem, &btHighScores.sprite, screen, &btHighScores.pos);
          SDL_BlitSurface(btSair.imagem, &btSair.sprite, screen, &btSair.pos);
     } else
          {
               SDL_BlitSurface(btVoltar.imagem, &btVoltar.sprite, screen, &btVoltar.pos);
          }
     SDL_BlitSurface(cursor.imagem, 0, screen, &cursor.pos);
     SDL_Flip(screen);
}

void carregaBotao(Botao * bt, char * arq, int x, int y, int qtdFrames)
{
     if(bt->imagem) SDL_FreeSurface(bt->imagem);
     bt->imagem = SDL_LoadBMP(arq);
     if(!bt->imagem)printf("Erro ao dar load na imagem %s.\n", arq);
     bt->pos.x = x;
     bt->pos.y = y;
     bt->pos.w = bt->imagem->w;
     bt->pos.h = bt->imagem->h;
     bt->sprite.x = 0;
     bt->sprite.y = 0;
     bt->sprite.w = bt->pos.w / qtdFrames;
     bt->sprite.h = bt->pos.h;
}

void carregaImagem(Imagem * imagem, char * arq, int x, int y)
{
     if(imagem->imagem) SDL_FreeSurface(imagem->imagem);
     imagem->imagem = SDL_LoadBMP(arq);
     if(!imagem->imagem) printf("Erro ao dar load na imagem %s\n", arq);
     SDL_SetColorKey(imagem->imagem, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 128, 128, 128));
     imagem->pos.x = x;
     imagem->pos.y = y;
     imagem->pos.w = imagem->imagem->w;
     imagem->pos.h = imagem->imagem->h;
}

int mouseOver(Botao * bt, int x, int y)
{
     if ((x > bt->pos.x) && (x < bt->pos.x + bt->sprite.w) &&
          (y > bt->pos.y) && (y < bt->pos.y + bt->sprite.h))
     {
          bt->emFoco = 1;
          bt->sprite.x = bt->sprite.w; 
         return 1;
     }
     
     bt->emFoco = 0;
     bt->sprite.x = 0; 
     return 0;
}
unsigned long int * carregaRecordes()
{
     char *nomeRecorde = "recordes.bin";
     FILE *recordes;
     unsigned long int *v, i;
     
     recordes = fopen(nomeRecorde, "rb");
     v =(unsigned long int*) malloc(TAM_RECORDE * sizeof(unsigned long int));
     if(!recordes) // Se o arquivo nao existir, ele ? criado com 0 em cada posi??o
     {         
          for(i=0; i<TAM_RECORDE; i++)
          {
               v[i] = 0;
          }
          
          recordes = fopen(nomeRecorde, "wb");
          
          for(i=0; i<TAM_RECORDE; i++)
          {
               fwrite(&v[i], sizeof(v[i]), 1, recordes);
          }
          fclose(recordes);
          recordes = fopen(nomeRecorde,"rb");
     }
     for (i=0;i<TAM_RECORDE;i++)
     {
          fread(&v[i], sizeof(v[i]), 1, recordes);
     }
     fclose(recordes);
     return v; 
}

void desenhaRecordes(unsigned long int * v)
{
     int i;
     char str[70]="bla";
     for (i=0;i<TAM_RECORDE;i++)
     {
          sprintf(str,"%d Lugar - %ld Pontos.",i+1,v[i]);
          desenhaTexto(str,320,160+50*i,0,0,0,fonte,0,0, fundoMenu.imagem);
     }
}

void desenhaTexto(char *str, int x, int y, int r, int g, int b, TTF_Font *fonte, int alpha, Uint8 alphaValor, SDL_Surface * tela)
{
     SDL_Surface *surf;
     SDL_Rect dest;
     SDL_Rect tmp;
     SDL_Color color;    

     color.r = r;
     color.g = g;
     color.b = b;

     surf = TTF_RenderText_Solid(fonte, str, color);

     dest.x = x;
     dest.y = y;
     dest.w = screen->w;
     dest.h = screen->h;
     tmp = dest;
     if (alpha)
     {
          SDL_SetAlpha(surf, SDL_SRCALPHA, alphaValor);
     }
     SDL_BlitSurface(surf, NULL, tela, &tmp);
     SDL_FreeSurface(surf);
}

void telaApresentacao(char str1[],char str2[])
{
     carregaImagem(&fundoMenu,str1, 0, 0);
     unsigned int i = 0, tempo = 0 , j = 0;
     do
     {
          if (SDL_GetTicks() > tempo)
          {
               if (j)
               {
                    i -= 5;
               }
               if ((i < 255) && (!j))
               {
                    i += 5;
               } else j = 1;
               tempo = SDL_GetTicks() +10;
          }
          SDL_FillRect(screen,NULL,0x000000);
          SDL_SetAlpha(fundoMenu.imagem,SDL_SRCALPHA,i);
          SDL_BlitSurface (fundoMenu.imagem,NULL,screen,NULL);
          SDL_Flip(screen);
     } while (i != 0);
     carregaImagem(&fundoMenu,str2, 0, 0);
}

void scoreTotal(unsigned long int score)
{
     Mix_Chunk *somPerdeu;
     somPerdeu = Mix_LoadWAV("audios/menu/somPerdeu.wav");
     Mix_PlayChannel(-1, somPerdeu, 0);
     telaApresentacao("imagens/menu/gameOver.bmp","imagens/menu/fundoMenu.bmp");
     adicionaRecorde(score);
     /*int i = 0;
     char temp[10];
     int clicou = 0;
     while (!clicou)
     {
          int x = 200, y = 150;
          mouseOver(&btVoltar, event.motion.x, event.motion.y);
          SDL_FillRect(screen,NULL,0x004040);
          desenhaTexto("SCORE GERAL",200,50,255,255,255,fonte,0,0);
          desenhaTexto("__________________________",200,100,255,255,255,fonte,0,0);
               for (i = 0; i < total; i++)
               {
                    desenhaTexto(vetor[i].str,x,y,255,255,255,fonte,0,0);
                    sprintf (temp,"%d",vetor[i].qtd);
                    desenhaTexto(temp,x+300,y,255,255,255,fonte,0,0);
                    sprintf (temp,"x%d",vetor[i].multp);
                    desenhaTexto(temp,x+400,y,255,255,255,fonte,0,0);
                    y += 50;
               }
               desenhaTexto("__________________________",x,y,255,255,255,fonte,0,0);
               y+=50;
               desenhaTexto("SCORE",x,y,255,255,255,fonte,0,0);
               sprintf (temp,"%d",score);
               desenhaTexto(temp,x+300,y,255,255,255,fonte,0,0);
               SDL_BlitSurface(btVoltar.imagem, &btVoltar.sprite, screen, &btVoltar.pos);
          SDL_BlitSurface(cursor.imagem,0,screen,&cursor.pos);
          SDL_Flip(screen);
          while ( SDL_PollEvent ( &event ) )
          {
                    switch(event.type) 
                    {
                         case SDL_QUIT:
                              exit(EXIT_SUCCESS);
                         break;
                         case SDL_MOUSEMOTION:
                              cursor.pos.x = event.motion.x;
                              cursor.pos.y = event.motion.y;
                         break;
                         case SDL_MOUSEBUTTONDOWN:
                              if (btVoltar.emFoco)
                              {
                                   clicou = 1;
                              }
                         break;
                    }
          }
     }*/
     Mix_FreeChunk(somPerdeu);
}

void adicionaRecorde(unsigned long int novaPontuacao)
{
     char *nomeRecorde = "recordes.bin";
     FILE *recordes;
     unsigned long int *v, i, j, temp;
     unsigned long int troca;
     recordes = fopen(nomeRecorde, "rb");
     v =(unsigned long int*) malloc(5 * sizeof(unsigned long int));
     for(i=0; i<5; i++)
     {
          fread(&v[i], sizeof(v[i]),1, recordes);
     }
     fclose(recordes);
     if(novaPontuacao > v[4])
     {
          v[4] = novaPontuacao;
          //ordena os recordes
          for (i = 4; i > 0; i--)
          {
              troca = 0;
              for (j = 0; j < i; j++)
              {
                  if (v[j] < v[j+1])
                  {
                      temp = v[j]; /* troca */
                      v[j] = v[j+1];
                      v[j+1] = temp;
                      troca = 1;
                  }
              }
              if (troca == 0)
                  break; /* nao houve troca */
             }
          // escreve no arquivo a nova relacao
          recordes = fopen(nomeRecorde, "wb");
          for(i=0; i<5; i++)
          {
               fwrite(&v[i], sizeof(v[i]), 1, recordes);
          }
          fclose(recordes);
     }
}

