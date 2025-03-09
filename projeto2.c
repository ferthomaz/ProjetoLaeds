#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX 50000

/****

LAED1 - Projeto - Estimativa do formato da pista

Alunos(as): Mariana Matias do Nacimento e Fernanda Moreira Thomaz 

Data: 27/01/2025

****/

/* ========================================================================= */
    

  typedef int TipoChave;

  typedef struct item
  {
    int Chave;
    int Tipo;
    int NumElementos;
    int PontoMedio;
  } TipoItem;

  typedef struct TipoCelula *TipoApontador;

  typedef struct TipoCelula {
    TipoItem Item;
    TipoApontador Prox;
  } TipoCelula;

  typedef struct {
    TipoApontador Primeiro, Ultimo;
  } TipoLista;

/* ========================================================================= */


  void FLVazia(TipoLista *Lista)
  { Lista -> Primeiro = (TipoApontador) malloc(sizeof(TipoCelula));
    Lista -> Ultimo = Lista -> Primeiro;
    Lista -> Primeiro -> Prox = NULL;
  }

  int Vazia(TipoLista Lista)
  { return (Lista.Primeiro == Lista.Ultimo);
  }

  TipoItem *cria_item(int Chave, int Tipo, int NumElementos, int PontoMedio){

      TipoItem *novoItem = (TipoItem *)malloc(sizeof(TipoItem));

      Chave++; //incrementa a chave que funciona como um id

      novoItem->Tipo = Tipo;
      novoItem->NumElementos = NumElementos;
      novoItem->PontoMedio = PontoMedio;

      return novoItem;

  }

  void Insere(TipoItem x, TipoLista *Lista){

    Lista -> Ultimo -> Prox = (TipoApontador) malloc(sizeof(TipoCelula));
    Lista -> Ultimo = Lista -> Ultimo -> Prox;
    Lista -> Ultimo -> Item = x;
    Lista -> Ultimo -> Prox = NULL;
  }

  void estimaFormato(int pontosMedios[], int i){

  if((pontosMedios[0] - pontosMedios[i-1]) > 50){       //limiar de 50 pra ser uma curva a direita
    printf("Resultado: Curva a direita.\n");
    return;
  }else if((pontosMedios[0] - pontosMedios[i-1]) < -50){    //limiar de -50 para curva a esquerda
    printf("Resultado: Curva a esquerda.\n");
    return;
  }

  printf("Resultado: Pista em linha reta.\n");

}

void impedimentoNaPista(TipoLista Lista){

    TipoApontador aux = Lista.Primeiro->Prox;

    //caso tenha alguma coisa diferente do segmento do meio, ele retorna que tem impedimento
    while (aux != NULL && aux->Prox != NULL) {
        if (aux->Item.Tipo == 1 &&
            aux->Prox != NULL && aux->Prox->Item.Tipo == 3 &&
            aux->Prox->Prox != NULL && aux->Prox->Prox->Item.Tipo == 2 &&
            aux->Prox->Prox->Prox != NULL && aux->Prox->Prox->Prox->Item.Tipo != 2 &&
            aux->Prox->Prox->Prox->Prox != NULL && aux->Prox->Prox->Prox->Prox->Item.Tipo == 2 ) {
            printf("Resultado: Pista com impedimento.\n");
            return;
            
        }
        aux = aux->Prox;
    }

    printf("Resultado: Pista sem impedimento.\n");
}

 void encontraPadrao(TipoLista Lista, int tamanho, int L) {
    TipoApontador aux = Lista.Primeiro->Prox;
    int contadora = 0;
    int i = 0;
   
    int pontosMedios[tamanho]; 


    while (aux != NULL && aux->Prox != NULL) {
        if (aux->Item.Tipo == 1 &&
            aux->Prox != NULL && aux->Prox->Item.Tipo == 3 &&
            aux->Prox->Prox != NULL && aux->Prox->Prox->Item.Tipo == 2 &&
            aux->Prox->Prox->Prox != NULL && aux->Prox->Prox->Prox->Item.Tipo == 3 &&
            aux->Prox->Prox->Prox->Prox != NULL && aux->Prox->Prox->Prox->Prox->Item.Tipo == 1) {
            contadora++;
            
            if(aux->Prox->Prox->Item.Tipo == 2){ //pois se achar padrao o aux sempre vai ser 1, entao tem que pegar o prox prox
              pontosMedios[i] = aux->Prox->Prox->Item.PontoMedio;  //coloca os pontos medios das linhas que possuem padrao em um vetor
              i++;
            }
            
        }
        aux = aux->Prox;
    }
    
    
    if(contadora >= L*0.7){     //se n for 0,7 nao da pra estimar, pois os dados so sao validos acima de 70 por cento
      estimaFormato(pontosMedios, i);
    }else{
      printf("Resultado: Formato da pista nao estimado.\n");
      return;
    }
    
}




int main(){
  TipoLista lista;
  TipoItem item;
  TipoApontador p;
  FLVazia(&lista);
  

int L; //quantidade de conjuntos na entrada formados por 950 numeros
scanf("%d", &L);


int n; //quantidade de pixels
int vet[MAX];
int j = 0;

for(int i = 0; i<L; i++){
  
  scanf("%d", &n);
// o elemento 5 é colocado para separar os grupo de 950 numeros
  for(int k = 0; k<n; k++){
    scanf("%d", &vet[j]);
    j++;
  }
  vet[j] = 5;
  j++;
}

int elementos[(n*L)+L];
int tam = 0;

elementos[0] = vet[0];
tam++;

int *ptr;
ptr = &vet[0];

elementos[0] = *ptr;

//separa os elementos do vetor sem repetir os valores
for (int i = 1; i <(n*L)+L; i++) {
        if (vet[i] != vet[i - 1]) { 
            elementos[tam] = vet[i];
            tam++;
        }
}
//calculo da quantidade e tipo dos segmentos
int quantidade[(n*L)+L];
int k = 0;

int tipo[(n*L)+L];
int x = 0;

int contadora = 0;

ptr = &vet[0];
ptr++;

for(; ptr != &vet[(n*L)+L]; ptr++){
  contadora++;
  if(*ptr != *(ptr - 1)){
    quantidade[k] = contadora;
    k++;
    contadora = 0;
  }
}

//vetor com as quantidades de algarismos em cada segmento
quantidade[k] = contadora+1;
k++;

//setando os tipos
for(int i = 0; i<tam; i++){
  if(elementos[i] == 0){
    tipo[i] = 1;
  }
  if(elementos[i] == 128){
    tipo[i] = 2;
  }
  if(elementos[i] == 255){
    tipo[i] = 3;
  }
  if(elementos[i] == 5){
    tipo[i] = 4;
  }
}

int pontoMedio[tam];
int *ptrPM;
ptrPM = &vet[1];
int contadoraPM = 0;
int aux = 0;
int f = 0;

//calculamos o ponto medio pegando o primeiro indice do segmento com o ultimo indice e fazendo a media disso 
for(; ptrPM != &vet[(n*L)+L]; ptrPM++){
  contadoraPM++;
  if(*ptrPM != *(ptrPM - 1)){
    pontoMedio[f] = (((contadoraPM-1) + aux) /2);
    if(*ptrPM==5){
      contadoraPM = 0;
      aux = contadoraPM;
    } else{
    aux = contadoraPM;
    }
    f++;
  }
}


for(int i = 0; i<tam-1; i++){
  TipoItem *novoItem = cria_item(i, tipo[i], quantidade[i], pontoMedio[i]);  
  Insere(*novoItem, &lista);
}
    
//para verificar o padrao
//encontraPadrao(lista, tam, L);

//para verificar impedimento na pista
impedimentoNaPista(lista);

return 0;
}
