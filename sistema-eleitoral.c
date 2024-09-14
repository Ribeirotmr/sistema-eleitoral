 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cadeiras 24.000 // constante responsavel por fixar o numero de cadeiras.
#define quant_mini 2  //quantidade minima para forma um partido

typedef struct
{
    /*A Estrutura Federa��o representa uma federa��o pol�tica, que � uma alian�a formal entre partidos
    que se unem para concorrer na elei��es ou atuar de forma conjunta. A estrutura Federacao armazena
    v�rias informa��es relacionadas a essa alian�a:*/

    char nome[110];  // Nome da Federa��o;
    char sigla[110]; // Sigla pertencente aquela federa��o;
    int votos;       // Numero total de votos que a federa��o possui;
    int quo_part;    // Quoeficiente partidario variavel responsavel pelo numero de cadeiras
    int afiliados;   // Variavel responsavel por armazenar a quantidade de partidos afiliados a federa��o
} Federacao;

typedef struct
{
    char nome[110];
    char sigla[110];
    int votos;
    Federacao *federacao;
    int quo_part;
} Partido;

typedef struct
{
    char nome[110];
    int idade;
    int votos;
    char numero5[100];
    Partido *partido;
    int vaga;

} Candidato;

typedef struct
{
    Partido partido;
    Candidato candidato;
    Federacao federacao;
} Voto;

int Arredonda(float valor) //ok
{
    /*Recebe um valor em float e retorna um inteiro arredondado    */

    int arredonda = valor;
    if ((valor - arredonda) > 0.5)
    {
        arredonda = arredonda + 1;
    }
    return arredonda;
}

int Verificar(char letra[]) 
{
    /*Recebe uma string e verifica se a String � somente letra e retorna 1,
    somente numeros no sistema decimal e retorna -1 e se n�o for nenhua das op��es
    anteriores retorna 0   */

    //Declara��o de Variaveis

    int i, ret = 0;
    int quant = strlen(letra);

    //Inicio

    for (i = 0; i < quant; i++)
    {
        if ((letra[i] >= 'a' && letra[i] <= 'z') || (letra[i] >= 'A' && letra[i] <= 'Z'))
        {
            ret = ret + 1;
        }
        if (letra[i] >= '0' && letra[i] <= '9')
        {
            ret = ret - 1;
        }
    }
    if (ret == quant)
    {
        return 1;
    }
    if (ret == (-1 * quant))
    {
        return -1;
    }
    return 0;
}

/*
essa funcao recebe um voto e um candidato e verifica se o voto � valido
se o voto for valido ele incrementa o numero de votos do candidato e do partido
e retorna 1, se o voto for invalido ele retorna 0
*/

int verificar_partido(Partido *partidos, int quantidade_partidos, char sigla_partido[],char nome[])
{
    for (int i = 0; i < quantidade_partidos; i++)
    {
        if (strcmp(partidos[i].sigla, sigla_partido) == 0 || strcmp(partidos[i].nome, nome) == 0 )
        {
            return 1;
        }
    }
    return 0;
}

/*
essa fun��o recebe um ponteiro para a estrutura Partido, um ponteiro para a estrutura Federacao, um ponteiro para a quantidade de partidos,
e um ponteiro para a quantidade de federacoes e retorna um ponteiro para a estrutura Partido. essa
fun��o � responsavel por cadastrar um partido, alocando um espa�o de memoria para um novo partido
e verificando se a sigla do partido � valida e se o partido ja foi cadastrado, caso a sigla seja invalida
ou o partido ja tenha sido cadastrado ele pede para o usuario digitar novamente a sigla do partido
e retorna o ponteiro para a estrutura Partido. ela retorna um ponteiro para a estrutura de partidos pois
� necessario alocar um novo espa�o de memoria para um novo partido.
*/

Partido *cadastro_partido(Partido *partidos, int *quantidade_partidos, int *q_partidos_sem_f )
{

    // Declara��o de Variaveis

    // Inicio

    partidos = (Partido *)realloc(partidos, (*quantidade_partidos + 1) * sizeof(Partido));

    if (partidos == NULL)
    {
        printf("Erro ao alocar memoria!!");
        exit(1);
    }
    strcpy(partidos[*quantidade_partidos].nome, "");
    strcpy(partidos[*quantidade_partidos].sigla, "");


    while (1)
    {   printf("Digite o nome do partido: \n");
        scanf("%100s", partidos[*quantidade_partidos].nome);
        printf("Digite a sigla do partido: \n");
        scanf("%5s", partidos[*quantidade_partidos].sigla);

        if ( Verificar(partidos[*quantidade_partidos].sigla) == 1 && Verificar(partidos[*quantidade_partidos].nome) == 1 )
        {

            if (verificar_partido(partidos, *quantidade_partidos, partidos[*quantidade_partidos].sigla,partidos[*quantidade_partidos].nome) == 0)
            {
                break;
            }
            else
            {
                printf("Partido ja cadastrado!\n");
            }
        }
        else
        {
            printf("Sigla ou nome invalido!\n");
        }
    }

    partidos[*quantidade_partidos].votos = 0;
    partidos[*quantidade_partidos].federacao = NULL;

    *quantidade_partidos += 1;
    *q_partidos_sem_f=*q_partidos_sem_f+1;

    return partidos;
}

int verificar_candidato(Candidato *candidato, int quantidade_candidatos, char sigla_candidato[]) 
{
    for (int i = 0; i < quantidade_candidatos; i++)
    {
        if (strcmp(candidato[i].numero5, sigla_candidato) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/*
Recebe um ponteiro para a estrutura Candidato, um ponteiro para a estrutura Partido, um ponteiro para a quantidade de candidatos
e um ponteiro para a quantidade de partidos, e retorna um ponteiro para a estrutura Candidato. essa fun��o � responsavel por cadastrar
um candidato, alocando um espa�o de memoria para um novo candidato e verificando se o numero do candidato � valido e se o candidato ja foi cadastrado
caso o numero do candidato seja invalido ou o candidato ja tenha sido cadastrado ele pede para o usuario digitar novamente o numero do candidato
e retorna o ponteiro para a estrutura Candidato. ela retorna um ponteiro pois � necessario alocar um novo espa�o de memoria para um novo candidato.
*/

Candidato *cadastro_candidato(Candidato *candidatos, Partido *partidos, int *quantidade_candidatos, int *quantidade_partidos)
{

    // Declara��o de Variaveis

    char sigla_partido[100],idade[10];
    int flag = 0;

    // Inicio

    candidatos = realloc(candidatos, (*quantidade_candidatos + 1) * sizeof(Candidato));

    if (candidatos == NULL)
    {
        printf("Erro ao alocar memoria!!");
        exit(1);
    }
    strcpy(candidatos[*quantidade_candidatos].nome, "");
    strcpy(candidatos[*quantidade_candidatos].numero5, "");
    printf("Digite o nome do Candidato: \n");
    scanf("%100s", candidatos[*quantidade_candidatos].nome);
    while(Verificar(candidatos[*quantidade_candidatos].nome) != 1){
        printf("Nome invalido\n");
        printf("Digite o nome do Candidato: \n");
        scanf("%100s", candidatos[*quantidade_candidatos].nome);
    }

    while (1)
        {
            printf("Digite a idade do Candidato: \n");
            scanf("%5s%*c", idade);
            if (Verificar(idade) == -1)
            {
                candidatos[*quantidade_candidatos].idade = strtol(idade, NULL, 10); // Transformando para n�mero inteiro de base 10
                break;
            }
            else
            {
                printf("Idade Invalida:\n ");
            }
        }
     //scanf("%d", &candidatos[*quantidade_candidatos].idade);



    while (1)
    {
        printf("Digite o numero do Candidato: \n");
        scanf("%50s", candidatos[*quantidade_candidatos].numero5);

        if (strlen(candidatos[*quantidade_candidatos].numero5) == 5 && Verificar(candidatos[*quantidade_candidatos].numero5) == -1)
        {
            if (verificar_candidato(candidatos, *quantidade_candidatos, candidatos[*quantidade_candidatos].numero5) == 0)
            {
                break;
            }
            else
            {
                printf("Candidato ja cadastrado!!\n");
            }
        }
        else
        {
            printf("Numero invalido!!\n");
        }
    }
    candidatos[*quantidade_candidatos].votos = 0;
    candidatos[*quantidade_candidatos].vaga = 0;

    while (flag == 0)
    {

        while (1)
        {
            printf("Digite a sigla do partido: \n");
            scanf("%5s", sigla_partido);

            if (Verificar(sigla_partido) == 1)
            {
                break;
            }
            else
            {
                printf("Sigla invalida!!\n");
            }
        }

        for (int i = 0; i < *quantidade_partidos; i++)
        {
            if (strcmp(partidos[i].sigla, sigla_partido) == 0)
            {
                candidatos[*quantidade_candidatos].partido = &partidos[i];
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            printf("Partido nao encontrado\n");
        }
    }

    *quantidade_candidatos += 1;
    return candidatos;
}

/*
Essa função Recebe um ponteiro para a estrutura Federacao, um inteiro quantidade_federacoes, uma string sigla_federacao e uma string nome
e retorna um inteiro. essa fun��o � responsavel por verificar se a federa��o ja foi cadastrada, caso a federa��o ja tenha sido cadastrada
ela retorna 1, caso contrario ela retorna 0
*/

int verificar_federacao(Federacao *federacoes, int quantidade_federacoes, char sigla_federacao[],char nome[])
{
    for (int i = 0; i < quantidade_federacoes; i++)
    {
        if (strcmp(federacoes[i].sigla, sigla_federacao) == 0 || strcmp(federacoes[i].nome, nome) == 0 )
        {
            return 1;
        }
    }
    return 0;
}

/*
Recebe um ponteiro para a estrutura Federacao, um ponteiro para a estrutura Partido, um ponteiro para a quantidade de federacoes,
e um ponteiro para a quantidade de partidos, e retorna um ponteiro para a estrutura Federacao. essa fun��o � responsavel por cadastrar
uma federacao, alocando um espa�o de memoria para uma nova federacao e verificando se a sigla da federacao � valida e se a federacao ja foi cadastrada
caso a sigla da federacao seja invalida ou a federacao ja tenha sido cadastrada ele pede para o usuario digitar novamente a sigla da federacao
e retorna o ponteiro para a estrutura Federacao. ele retorna um ponteiro pois � necessario alocar um novo espa�o de memoria para uma nova federacao.
*/

Federacao *cadastro_federacao(Federacao *federacoes, Partido *partidos, int *quantidade_federacoes, int *quantidade_partidos,int *q_partidos_sem_f  )
{

    // Declara��o de Variaveis

    int quant = 0, n = 0;
    short partidoEncontrado = 0;
    char sigla_federacao[100];

    // Inicio
    federacoes = (Federacao *)realloc(federacoes, (*quantidade_federacoes + 1) * sizeof(Federacao));

    if (federacoes == NULL)
    {
        printf("Erro ao alocar memoria!!");
        exit(1);
    }
    strcpy(federacoes[*quantidade_federacoes].nome, "");
    strcpy(federacoes[*quantidade_federacoes].sigla, "");

   while (1)
    {   printf("Digite o nome da federacao: \n");
        scanf("%100s", federacoes[*quantidade_federacoes].nome);
        printf("Digite a sigla da federacao: \n");
        scanf("%5s", federacoes[*quantidade_federacoes].sigla);

        if (Verificar(federacoes[*quantidade_federacoes].sigla) == 1 && Verificar(federacoes[*quantidade_federacoes].nome) == 1 )
        {
            if (verificar_federacao(federacoes, *quantidade_federacoes, federacoes[*quantidade_federacoes].sigla,federacoes[*quantidade_federacoes].nome) == 0)
            {
                break;
            }
            else
            {
                printf("Federacao ja cadastrada!\n");
            }
        }
        else
        {
            printf("Sigla ou nome invalido!!\n");
        }
    }

    federacoes[*quantidade_federacoes].votos = 0;
    federacoes[*quantidade_federacoes].afiliados = 0;

    //enquano numero afiliados decrementado for zero, fa�a mostre a quantidade de vezes para cadastrar o partido

    while (quant < quant_mini || quant > *q_partidos_sem_f)
    {
        printf("Digite a quantidade de Partidos que deseja se afiliar: ");
        scanf("%d", &quant);
        if (quant < quant_mini || quant > *q_partidos_sem_f)
        {
            printf("E necessario ter pelo menos dois partidos afiliados a federacao ou existir a quantidade desejada.\n");
        }
    }

    while (n < quant)
    {
        while (1)
        {
            printf("Digite a sigla do partido: ");
            scanf("%5s", sigla_federacao);

            if (Verificar(sigla_federacao) == 1)
            {
                break;
            }
            else
            {
                printf("Sigla invalida!!\n");
            }
        }

        for (int i = 0; i < *quantidade_partidos; i++)
        {
            if (strcmp(partidos[i].sigla, sigla_federacao) == 0)
            {
                if (partidos[i].federacao != NULL) // erro "concertado"
                {
                    printf("Partido ja associado a uma federacao!\n");
                    break;
                }
                n += 1;
                *q_partidos_sem_f=*q_partidos_sem_f-1;

                partidos[i].federacao = &federacoes[*quantidade_federacoes];
                partidoEncontrado = 1;
                break;
            }

        }

        if (partidoEncontrado == 0)
        {
            printf("Partido nao encontrado!\n");
        }

        partidoEncontrado = 0;

    }
    *quantidade_federacoes=*quantidade_federacoes+1;
    return federacoes;
}



/*
A fun��o registrar voto Recebe um ponteiro para a estrutura Partido, um ponteiro para a estrutura Candidato, 
um ponteiro para a estrutura Federacao um ponteiro para a quantidade de federacoes,
um ponteiro para a quantidade de candidatos, um ponteiro para a quantidade de partidos, um ponteiro para a quantidade total de votos, um ponteiro para a quantidade de votos brancos
e um ponteiro para a quantidade de votos validos e n�o retorna nada. essa fun��o � responsavel por registrar um voto, verificando se o candidato existe
e se o voto � valido, caso o candidato exista e o voto seja valido ele incrementa o numero de votos do candidato, do partido e da federacao
e incrementa o numero total de votos e o numero de votos validos, caso o candidato n�o exista ou o voto seja invalido ele incrementa o numero de votos brancos
e o numero total de votos
*/

void registrar_voto(Partido *partidos, Candidato *candidatos, Federacao *federacoes, int *quantidade_federacoes, int *quantidade_candidatos,
                    int *quantidade_partidos, int *quantidade_total_votos, int *quantidade_votos_brancos, int *quantidade_votos_validos)
{

    /*Fun��o registra_voto, fun��o resposavel por fazer a contagem
    de todos os votos registrados e quantidade de votos em brancos*/

    // Declara��o de Variaveis
    int j, i, z;
    int op, verifica = 1;
    char numero_candidato[50];

    printf("\n\nDigite o numero do candidato: ");
    scanf("%10s", numero_candidato);

    for (i = 0; i < *quantidade_candidatos; i++)
    {

        if (strcmp(candidatos[i].numero5, numero_candidato) == 0)
        {
            verifica = 0;
            printf("\nCandidato encontrado!!");

            printf("\nNome: %s", candidatos[i].nome);

            do
            {
                printf("\nConfirmar voto");
                printf("\n1-Sim\n");
                printf("\n2-Retornar\n");
                printf("\n3-Votar em branco\n");
                scanf("%d", &op);
            } while (op != 1 && op != 2 && op != 3);

            switch (op)
            {
            case 1:
                candidatos[i].votos += 1;
                for (j = 0; j < *quantidade_partidos; j++)
                {
                    if (strcmp(candidatos[i].partido->sigla, partidos[j].sigla) == 0)
                    {
                        partidos[j].votos += 1;
                         if (partidos[j].federacao != NULL)
                        {

                        for (z = 0; z < *quantidade_federacoes; z++)
                        {
                            if (strcmp(partidos[j].federacao->sigla, federacoes[z].sigla) == 0)
                            {
                                federacoes[z].votos += 1;
                                break;
                            }
                        }

                        }
                    }
                }
                *quantidade_total_votos += 1;
                (*quantidade_votos_validos) += 1;
                printf("\nVoto registrado com sucesso!!");
                break;

            case 2:
                printf("\nVoce sera redirecionado para o menu de votacao!!!");
                break;

            case 3:
                *quantidade_total_votos += 1;
                *quantidade_votos_brancos += 1;
                printf("\nVoto registrado com sucesso!!");
                break;
            }
        }

    }
    if (verifica == 1)
    {

        printf("\nVOTO NULO "); 

        do
        {
                printf("\nConfirmar voto");
                printf("\n1-Sim\n");
                printf("\n2-Retornar\n");
                printf("\n3-Votar em branco\n");
            scanf("%d", &op);

        } while (op != 1 && op != 2 && op != 3);

        switch (op)
        {
        case 1:
            *quantidade_total_votos += 1;
            printf("\nVOTO NULO!!");
            printf("\nVoto registrado com sucesso!!");
            break;

        case 2:
            printf("\nVoce sera redirecionado para o menu de votacao!!!");
            break;

        default:
            *quantidade_total_votos += 1;
            *quantidade_votos_brancos += 1;
            printf("\nVoto registrado com sucesso!!");
        }
    }
}

/*
A fun��o secao1
Recebe um inteiro quantidade_total_votos, um inteiro quantidade_votos_brancos, um inteiro quantidade_votos_validos e um inteiro quo_eleito
e n�o retorna nada. essa fun��o � responsavel por mostrar a quantidade total de votos, a quantidade de votos validos, a quantidade de votos nulos
a quantidade de votos em branco e o quociente eleitoral
*/

void secao1(int quantidade_total_votos, int quantidade_votos_brancos, int quantidade_votos_validos, int quo_eleito) //ok
{
    printf("\nNumero total de votos: %d", quantidade_total_votos);
    printf("\nNumero de votos validos: %d", quantidade_votos_validos);
    printf("\nNumero de votos nulos: %d", quantidade_total_votos - (quantidade_votos_validos + quantidade_votos_brancos));
    printf("\nNumero de votos em branco: %d", quantidade_votos_brancos);
    printf("\nQuociente eleitoral: %d", quo_eleito);
}

/*
A fun��o secao2
Recebe um ponteiro para a estrutura Candidato, um inteiro quantidade_candidatos
e n�o retorna nada. essa fun��o � responsavel por mostrar o nome do candidato, o numero do candidato, o numero de votos do candidato
e o partido do candidato
*/

void secao2(Candidato *candidatos, int quantidade_candidatos) 
{
    int i;

    for (i = 0; i < quantidade_candidatos; i++)
    {

        printf("\nCandidado: %s", candidatos[i].nome);
        printf("\nNumero: %s", candidatos[i].numero5);
        printf("\nNumero de votos: %d", candidatos[i].votos);
        printf("\nPartido: %s", candidatos[i].partido->nome);
    }
}

/*
A fun��o secao3
Recebe um ponteiro para a estrutura Partido, um inteiro quantidade_partidos, um ponteiro para a estrutura Federacao e um inteiro quantidade_federacoes
e n�o retorna nada. essa fun��o � responsavel por mostrar o nome do partido, a sigla do partido e o total de votos do partido
e o nome da federacao, a sigla da federacao e o total de votos da federacao
*/

void secao3(Partido *partidos, int quantidade_partidos, Federacao *federacoes, int quantidade_federacoes) //ok
{
    int i;

    for (i = 0; i < quantidade_partidos; i++)
    {

        if (partidos[i].federacao == NULL)
        {
            printf("\nPartido: %s", partidos[i].nome);
            printf("\nSigla: %s", partidos[i].sigla);
            printf("\nTotal de votos: %d", partidos[i].votos);
        }
    }
    if(quantidade_federacoes>0){
    for (i = 0; i < quantidade_federacoes; i++)
    {
        printf("\nFederacao: %s", federacoes[i].nome);
        printf("\nSigla: %s", federacoes[i].sigla);
        printf("\nTotal de votos: %d", federacoes[i].votos);
    }
    }
}

/*
A fun��o secao4 Recebe um ponteiro para a estrutura Partido, um inteiro quantidade_partidos, um ponteiro para a estrutura Federacao e um inteiro quantidade_federacoes
e n�o retorna nada.essa fun��o � responsavel por mostrar o nome do partido, a sigla do partido e o quociente partidario do partido
e o numero de cadeiras do partido
e o nome da federacao, a sigla da federacao e o quociente partidario da federacao
e o numero de cadeiras da federacao
*/
void secao4(Partido *partidos, Federacao *federacoes, int quantidade_partidos, int quantidade_federacoes)
{
    int i = 0;

    for (i = 0; i < quantidade_partidos; i++)
    {
        if (partidos[i].federacao == NULL)
        {
            printf("\nPartidos: ");
            printf("\nPartido: %s", partidos[i].nome);
            printf("\nSigla: %s", partidos[i].sigla);
            printf("\nQuociente Partid�rio:%d", partidos[i].quo_part);
            printf("\nNumero de cadeiras: %d", partidos[i].quo_part);
        }
    }
    if(quantidade_federacoes>0){
    for (i = 0; i < quantidade_federacoes; i++)
    {
        printf("\n\nFederacoes: ");
        printf("\nFederacao: %s", federacoes[i].nome);
        printf("\nSigla: %s", federacoes[i].sigla);
        printf("\nQuociente Partidario: %d", federacoes[i].quo_part);
        printf("\nNumero de cadeiras: %d", federacoes[i].quo_part);
    }
    }

}

/*
A fun��o secao5
Recebe um ponteiro para a estrutura Partido, um inteiro quantidade_partidos, um ponteiro para a estrutura Federacao,inteiro quantidade_federacoes,
inteiro quantidade_candidatos,um ponteiro para a estrutura Candidato,inteiro quo_eleitor, inteiro ponteiro sobra e n�o retorna nada.
Imprimir o nome completo do candidato eleito, o nome do partido a qual ele faz parte e seus votos
*/

void secao5(Partido *partidos, Candidato *candidatos, Federacao *federacoes, int quantidade_partidos, int quantidade_candidatos, int quantidade_federacoes, int *sobra)

{
    int eleito = 0,idade=0,i, j, a = 0, s,maior = 0,cont=0;

    for (j = 0; j < quantidade_partidos; j++)
    {
        if (partidos[j].federacao == NULL && partidos[j].quo_part > 0)
        {
            do
            {   maior = 0;
                idade=0;
                 cont=0;
                for (i = 0; i < quantidade_candidatos; i++)
                {
                    if (strcmp(candidatos[i].partido->sigla, partidos[j].sigla) == 0 && candidatos[i].vaga == 0)
                    {
                        if (candidatos[i].votos > maior || (candidatos[i].votos == maior && candidatos[i].idade > idade))
                        {
                            maior = candidatos[i].votos;
                            idade=candidatos[i].idade;
                            a = i;
                            cont=1;
                        }
                    }
                }
                if ( cont>0 )
                {
                    partidos[j].quo_part = partidos[j].quo_part - 1;
                    candidatos[a].vaga = 1;
                    eleito = eleito + 1;
                    printf("\n--------------------------------------------------");
                    printf("\nCandidato: %s", candidatos[a].nome);
                    printf("\nPartido: %s", candidatos[a].partido->nome);
                    printf("\nVotos: %d", candidatos[a].votos);
                }

                else
                {
                    break;
                }

            } while (partidos[j].quo_part > 0);

            *sobra = *sobra - eleito;
            eleito = 0;
            //  }
        }
    }
    if(quantidade_federacoes>0){
    for (s = 0; s < quantidade_federacoes; s++)
    {
        if (federacoes[s].quo_part > 0)
        {
            do
            {   maior = 0;
                idade=0;
                cont=0;
                for (j = 0; j < quantidade_partidos; j++)
                {
                    if (partidos[j].federacao != NULL)
                    {
                        if (strcmp(partidos[j].federacao->sigla, federacoes[s].sigla) == 0)
                        {
                            for (i = 0; i < quantidade_candidatos; i++)
                            {
                                if (strcmp(candidatos[i].partido->sigla, partidos[j].sigla) == 0 && candidatos[i].vaga == 0)
                                {
                                    if (candidatos[i].votos > maior || (candidatos[i].votos == maior && candidatos[i].idade > idade))
                                    {
                                        maior = candidatos[i].votos;
                                        idade=candidatos[i].idade;
                                        a = i;
                                        cont=1;
                                    }
                                }
                            }
                        }
                    }
                }

                 if ( cont > 0 )
                {
                    federacoes[s].quo_part = federacoes[s].quo_part - 1;
                    eleito = eleito + 1;
                    candidatos[a].vaga = 1;

                    printf("\nCandidato: %s", candidatos[a].nome);
                    printf("\nPartido: %s", candidatos[a].partido->nome);
                    printf("\nVotos: %d", candidatos[a].votos);
                }
                else
                {
                    break;
                }


            } while (federacoes[s].quo_part > 0);
            *sobra = *sobra - eleito;
            eleito = 0;
        }
    }

    }


}
/* A fun��o secao6
Recebe inteiro quantidade_candidatos,um ponteiro para a estrutura Candidato e n�o retorna nada.
Imprimir o nome completo do candidato suplente em ordem decresente  , 
o nome do partido a qual ele faz parte e seus votos
*/


void secao6(Candidato *candidatos, int quantidade_candidatos, int sobra){
    int maior = 0, a,idade=0,cont=0;
    printf("\nSobras: %d", sobra);
    do{
        maior = 0;
        idade=0;
        cont=0;
        for (int i = 0; i < quantidade_candidatos; i++){
            if (candidatos[i].vaga == 0){
                if (candidatos[i].votos > maior || (candidatos[i].votos == maior 
                && candidatos[i].idade > idade)){
                    maior = candidatos[i].votos;
                    idade=candidatos[i].idade;
                    a = i;
                }
                cont=1;
            }
        }
        if (cont > 0){
            printf("\nCandidato: %s", candidatos[a].nome);
            printf("\nPartido: %s", candidatos[a].partido->nome);
            printf("\nVotos: %d", candidatos[a].votos);
            candidatos[a].vaga = -1;
        }

        else{
            printf("\nFIM DA LISTA");
        }

    } while (cont > 0);
}

int main()
{

    /*Função main, fun�ao reposavel pelo MENUS e chamada de todas as fun��es ultilizadas*/

    // Declara��es de Variaveis

    int quantidade_partidos = 0, quantidade_candidatos = 0, quantidade_federacoes = 0,
    q_partidos_sem_f=0; 
    // Declara��o da varaivel que ira contar o numero de cadastros realizados de cada estrutura;
    char opcao1[10]; 
    // Variavel Responsavel por armazenar a op��o que sera escolhida pela pessoa
    int opcao_int, i, sobra = cadeiras;
    int quantidade_total_votos = 0, quantidade_votos_brancos = 0, quantidade_votos_validos = 0;
    int quo_eleito=0;

    /*
    Declara��o de ponteiros para as estruturas Partido,
    Candidato e Federacao, que ser�o responsaveis por armazenar os cadastros realizados
    isso serve para
    evitar a declara��o de cada variavel de forma individual
    Declara��o de ponteiros para as estruturas.
    isso serve para
    que o programa possa acessar as estruturas
    de forma mais facil e rapida
    */
    
    Partido *partidos = NULL;
    Candidato *candidatos = NULL;
    Federacao *federacoes = NULL;

    // MENU DE CADASTRO E CHAMADAS DE FUN��ES PARA CADASTRO;

    do
    {
        printf("\n");
        printf("+----------------------------------+\n");
        printf("|          URNA ELETRONICA         |\n");
        printf("+----------------------------------+\n");
        printf("|        MENU DE CADASTRO          |\n");
        printf("+----------------------------------+\n");
        printf("| 1 - Cadastrar Partido            |\n");
        printf("| 2 - Cadastrar Candidato          |\n");
        printf("| 3 - Cadastrar Federacao          |\n");
        printf("| 4 - Terminar cadastro            |\n");
        printf("+----------------------------------+\n");
        printf("\n");

        // VER ESSA PARTE

        while (1)
        {
            printf("\nDigite uma opcao: \n");
            scanf("%5s%*c", opcao1);
            if (Verificar(opcao1) == -1)
            {
                opcao_int = strtol(opcao1, NULL, 10); 
                // Transformando para n�mero inteiro de base 10
                break;
            }
            else
            {
                printf("\nOpcao invalida: ");
            }
        }
        switch (opcao_int)
        {
        case 1:

            partidos = cadastro_partido(partidos, &quantidade_partidos, &q_partidos_sem_f);

            printf("\nPartido cadastrado com sucesso !!");
            break;
        case 2:
            if (quantidade_partidos == 0)
            {
                printf("\nNenhum partido cadastrado!");
                break;
            }
            candidatos = cadastro_candidato(candidatos, partidos, 
            &quantidade_candidatos, &quantidade_partidos);

            printf("\nCandidato Cadastrado com sucesso!!");
            break;
        case 3:
            if (q_partidos_sem_f > 1)
            {
                federacoes = cadastro_federacao(federacoes, partidos, &quantidade_federacoes, 
                &quantidade_partidos,  &q_partidos_sem_f);

                printf("\nFederacao Cadastrada com sucesso!!");
            }
            else
            {
                printf("\nNecessario que pelo menos 2 partidos disponiveis!!!");
            }
            break;
        case 4:
            if (quantidade_candidatos == 0)
            {
                opcao_int = 0;
                printf("\nNenhum candidato cadastrado!!\n");
                break;
            }
            printf("\nCadastro finalizado\n");
            break;
        default:
            printf("Opcao invalida\n");
        }
    } while (opcao_int != 4);
    opcao_int = 0;

    // MENU PARA VOTA��O.

    do
    {
        printf("\n");
        printf("+-----------------------------------+\n");
        printf("|           MENU DE VOTACAO         |\n");
        printf("+-----------------------------------+\n");
        printf("| 1 - Registrar um novo voto        |\n");
        printf("| 2 - Encerrar votacao              |\n");
        printf("+-----------------------------------+\n");

        while (1)
        {
            printf("\nDigite uma opcao: \n");
            scanf("%5s%*c", opcao1);
            if (Verificar(opcao1) == -1)
            {
                opcao_int = strtol(opcao1, NULL, 10); 
                // Transformando para n�mero inteiro de base 10
                break;
            }
            else
            {
                printf("\nOpcao invalida: ");
            }
        }

        switch (opcao_int)
        {

        case 1:
            registrar_voto(partidos, candidatos, federacoes, &quantidade_federacoes, 
            &quantidade_candidatos, &quantidade_partidos, &quantidade_total_votos, 
            &quantidade_votos_brancos, &quantidade_votos_validos);
            break;
        case 2:
            if(quo_eleito==0 ){

            printf("\nValor insuficiente de votos !!");
            break;

        }
            printf("\nSistema de votacao encerrado!!");
            break;
        default:
            printf("\nOpcao invalida!!");
        }
        quo_eleito = Arredonda(quantidade_votos_validos / cadeiras);



    } while (opcao_int != 2 || quo_eleito==0 );

    // RELATORIO FINAL URNA ELEITORAL.


    for (i = 0; i < quantidade_partidos; i++)
    {
        if (partidos[i].federacao == NULL)
        {
            partidos[i].quo_part = partidos[i].votos / quo_eleito;
        }
    }

    if (quantidade_federacoes > 0)
    {
        for (i = 0; i < quantidade_federacoes; i++)
        {
            federacoes[i].quo_part = federacoes[i].votos / quo_eleito;
        }
    }

    printf("\n\nRELATORIO!!");
    printf("\nSecao 1 - Numero de votos!!\n");
    secao1(quantidade_total_votos, quantidade_votos_brancos, 
    quantidade_votos_validos, quo_eleito);

    printf("\n\n--------------------------------------------------");

    printf("\nSecao 2 - Votos por candidato!!\n");
    secao2(candidatos, quantidade_candidatos);

    printf("\n\n--------------------------------------------------");

    printf("\nSecao 3 - Votos de Partidos/Federacao");
    secao3(partidos, quantidade_partidos, federacoes, quantidade_federacoes);

    printf("\n\n--------------------------------------------------");

    printf("\nSecao 4 - Tabela quociente partidario de cada partido/federacao e o n�mero de cadeiras");
    secao4(partidos, federacoes, quantidade_partidos, quantidade_federacoes);

    printf("\n\n--------------------------------------------------");

    printf("\nSecao 5 - Candidatos eleitos");
    secao5(partidos, candidatos, federacoes, quantidade_partidos, 
    quantidade_candidatos, quantidade_federacoes, &sobra);

    printf("\n\n--------------------------------------------------");

    printf("\nSecao 6 - Candidatos suplentes");
    secao6(candidatos, quantidade_candidatos,sobra);

    printf("\n\n--------------------------------------------------");

    free(partidos);
    free(candidatos);
    free(federacoes);
}
