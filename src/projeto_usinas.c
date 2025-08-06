/******************************************************************************
Aluno(a): Davi Marques do Egito Coelho
Matrícula: 231030421
Declaro que eu sou o(a) autor(a) deste trabalho e que o código não foi copiado
de outra pessoa nem repassado para alguém. Estou ciente de que poderei sofrer
penalizações na avaliação em caso de detecção de plágio.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#define TAM_MAX_RA 100 //Caracteres a serem usados para identificar nominalmente a RA
#define TAM_MAX_CNPJ 19 //São 14 dígitos + NUL character
#define TAM_MAX_ENERGIA 8 //São 7 caracteres do número máx, 50000, + 1 NUL [barra 0]
#define TAM_MAX_CPF 15
#define TAM_MAX_NOME 100
#define TAM_MAX_STATUS 100 //A quantidade máxima de caracteres para definir o status da miniusina
#define TAM_MAX_DATA 11
#define TAM_MAX_ID 100 //2 do número (porque são menos de 99 no DF) + NUL
#define TAM_MAX_VALOR_DIGITADO 19 //Quando não se sabe se o usuário vai inserir CPF ou CNPJ
#define TAM_MAX_QUANT_MINIUSINAS 100
#define TAM_MAX_QUANT_RAS 100
#define TAM_MAX_QUANT_CONTRATOS 100

//Struct de miniusinas
struct miniusinas {
    char cnpj_miniusina[TAM_MAX_CNPJ];
    char nome_miniusina[TAM_MAX_NOME];
    char capacidade_total_miniusina[TAM_MAX_ENERGIA];
    char ra_miniusina[TAM_MAX_RA];
    char status_miniusina[TAM_MAX_STATUS];
    };

//Struct de consumidores - mostrar os dados e contratos vigentes que ele tem
struct consumidores {
    char cnpj_consumidor[TAM_MAX_CNPJ];
    char cpf_consumidor[TAM_MAX_CPF];
    char nome_consumidor[TAM_MAX_NOME];
    char ra_consumidor[TAM_MAX_RA];
};

//Struct de RAs
struct ra {
    char id_ra[TAM_MAX_ID];
    char nome_ra[TAM_MAX_NOME];
    int contagem_miniusinas; //Não está no arquivo original, mas criei para poder contar para alguns dos itens do menu
    int contagem_contratos;
    float capacidade_total;
    float energia_contratada;
    float percentual_contratado;
};

//Struct de Contratos
struct contratos {
    char id_contrato[TAM_MAX_ID];
    char id_consumidor[TAM_MAX_ID];
    char cnpj_miniusina[TAM_MAX_CNPJ];
    char data_inicio_contrato[TAM_MAX_DATA];
    char energia[TAM_MAX_ENERGIA];
};

//O que será digitado na tela enquanto a opção for diferente da 9, que é sair
void menu() {
    printf("\n*** Por favor, digite o número correspondente ao serviço desejado: \n");
    printf("1 - Consultar miniusina cadastrada\n");
    printf("2 - Consultar consumidor cadastrado\n");
    printf("3 - Listar as miniusinas cadastradas\n");
    printf("4 - Listar as miniusinas em operação por ordem decrescente de capacidade de geração de energia\n");
    printf("5 - Listar as RAs por ordem decrescente de quantidade de miniusinas\n");
    printf("6 - Listar as RAs por ordem crescente de número de contratos\n");
    printf("7 - Listar as RAs por ordem decrescente de capacidade de geração\n");
    printf("8 - Listar as RAs por ordem decrescente de percentual de energia disponível\n");
    printf("9 - Sair do programa\n");
    printf("Número: ");
}

//Void para verificar se o CNPJ da miniusina corresponde a uma cadastrada ou não - Opção 1 do Menu
void consultar_miniusina() {
    FILE *miniusinas; //representa o arquivo
    FILE *contratos;
    struct miniusinas miniusina_cadastrada; //Vamos usar o miniusina_cadastrada para guardar a informação contida no arquivo posteriormente
    struct contratos contrato_consultado;
    char cnpj_consulta[TAM_MAX_CNPJ];
    int miniusina_encontrada = 0; //Indica se há alguma miniusina com o CNPJ digitado pelo usuário
    int contrato_encontrado = 0; //Indica se o CNPJ tem um contrato cadastrado ou não
    //Primeiramente, guardar o CNPJ digitado para posterior verificação
    printf("Digite o CNPJ que você quer consultar: ");
    scanf("%s", cnpj_consulta);
    //Agora, início do procedimento para verificar se é válido ou não
    miniusinas = fopen("miniusinas.csv", "r"); //r porque será somente leitura dos dados
    //Para termos certeza que o arquivo foi devidamente aberto:
    if (miniusinas != NULL) {
        //Para descartar o cabeçalho do arquivo, criaremos uma varíável string
        //Desta forma, ela será armazenada e nada será feito com ela
        char primeira_linha[1000];
        fscanf(miniusinas, "%[^\n]\n", primeira_linha);
        //Estrutura de repetição para ler cada linha do arquivo
        while (!feof(miniusinas)) {
            fscanf(miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
            miniusina_cadastrada.cnpj_miniusina,
            miniusina_cadastrada.nome_miniusina,
            miniusina_cadastrada.capacidade_total_miniusina,
            miniusina_cadastrada.ra_miniusina,
            miniusina_cadastrada.status_miniusina);
            //Caso o cnpj digitado corresponda a um CNPJ dentro do arquivo de miniusinas, todos os dados relevantes serão printados
            if (strcmp(cnpj_consulta, miniusina_cadastrada.cnpj_miniusina) == 0) {
                printf("\nCNPJ: %s\n", miniusina_cadastrada.cnpj_miniusina);
                printf("Nome: %s\n", miniusina_cadastrada.nome_miniusina);
                printf("Capacidade total: %s\n", miniusina_cadastrada.capacidade_total_miniusina);
                printf("RA: %s\n", miniusina_cadastrada.ra_miniusina);
                printf("Status: %s\n", miniusina_cadastrada.status_miniusina);
                miniusina_encontrada = 1;
            }
        }
    } if (miniusina_encontrada == 1) {
                //Agora mostrar os contratos se houver miniusinas
                contratos = fopen("contratos.csv", "r");
                if (contratos != NULL) {
                    //Para descartar o cabeçalho do arquivo, criaremos uma varíável string
                    //Desta forma, ela será armazenada e nada será feito com ela
                    char primeira_linha[1000];
                    fscanf(contratos, "%[^\n]\n", primeira_linha);
                    //Agora, um loop para escanear cada linha do contrato
                    while (!feof(contratos)) {
                        fscanf(contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                        contrato_consultado.id_contrato,
                        contrato_consultado.id_consumidor,
                        contrato_consultado.cnpj_miniusina,
                        contrato_consultado.data_inicio_contrato,
                        contrato_consultado.energia);
                        //Caso o cnpj digitado corresponda a um CNPJ dentro do arquivo de contratos, todos os dados em contratos serão printados
                        if (strcmp(cnpj_consulta, contrato_consultado.cnpj_miniusina) == 0) {
                            contrato_encontrado = 1;
                            printf("\nID do Contrato: %s\n", contrato_consultado.id_contrato);
                            printf("ID do Consumidor: %s\n", contrato_consultado.id_consumidor);
                            printf("CNPJ da Miniusina: %s\n", contrato_consultado.cnpj_miniusina);
                            printf("Data do início do contrato: %s\n", contrato_consultado.data_inicio_contrato);
                            printf("Energia contratada: %s\n", contrato_consultado.energia);
                        }
                    }
                } if (contrato_encontrado == 0) {
                        //Caso nenhum contrato tenha sido encontrado
                        printf("\nContrato não encontrado para o CNPJ digitado.\n");
                    }
    } if (miniusina_encontrada == 0) {
            //Caso nenhuma miniusina tenha sido encontrada
            printf("\nCNPJ digitado não corresponde a nenhuma miniusina. Por favor, verifique os dígitos e tente novamente.\n");
            }
    fclose(miniusinas);
    fclose(contratos);
}

//void para o item número 2 do menu, que é a consulta do cadastro dos consumidores
void consultar_consumidor() {
    FILE *consumidores;
    FILE *contratos;
    struct consumidores consumidores_cadastrados;
    struct contratos contrato_consultado;
    char cpf_consulta[TAM_MAX_CPF];
    char cnpj_consulta[TAM_MAX_CNPJ];
    char opcao_cnpj_cpf[TAM_MAX_CNPJ];
    int consumidor_encontrado = 0; //Indica se o CPF ou CNPJ forem válidos para um consumidor
    int contrato_encontrado = 0; //Indica se o CNPJ tem um contrato cadastrado ou não
    //Armazenar o valor
    printf("Você prefere digitar o CPF ou CNPJ do consumidor? \n");
    scanf("%s", opcao_cnpj_cpf);
    //Agora, verificar se aquilo que foi digitado corresponde a CPF/cpf
    if(strcmp(opcao_cnpj_cpf, "CPF") == 0 || strcmp(opcao_cnpj_cpf, "cpf") == 0) {
        //Agora pedir para digitar o CPF sabendo que o número digitado será este.
        printf("Digite o CPF: ");
        scanf("%s", cpf_consulta);
        //Hora de abrir o arquivo
        consumidores = fopen("consumidores.csv", "r");
        if (consumidores != NULL) {
        char primeira_linha[1000];
        fscanf(consumidores, "%[^\n]\n", primeira_linha);
        while (!feof(consumidores)) {
            fscanf(consumidores, "%[^;];%[^;];%[^\n]\n",
            consumidores_cadastrados.cpf_consumidor,
            consumidores_cadastrados.nome_consumidor,
            consumidores_cadastrados.ra_consumidor);
            //Caso o CPF digitado corresponda a um CPF dentro do arquivo de consumidores, todos os dados de consumidores serão printados
            if (strcmp(cpf_consulta, consumidores_cadastrados.cpf_consumidor) == 0) {
                printf("\nCPF: %s\n", consumidores_cadastrados.cpf_consumidor);
                printf("Nome: %s\n", consumidores_cadastrados.nome_consumidor);
                printf("RA: %s\n", consumidores_cadastrados.ra_consumidor);
                consumidor_encontrado = 1;
            }
        }
        } if (consumidor_encontrado == 1) {
                //Agora mostrar os contratos se houver registro de miniusinas
                contratos = fopen("contratos.csv", "r");
                if (contratos != NULL) {
                    char primeira_linha[1000];
                    fscanf(contratos, "%[^\n]\n", primeira_linha);
                    while (!feof(contratos)) {
                        fscanf(contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                        contrato_consultado.id_contrato,
                        contrato_consultado.id_consumidor,
                        contrato_consultado.cnpj_miniusina,
                        contrato_consultado.data_inicio_contrato,
                        contrato_consultado.energia);
                        //Caso o CPF digitado corresponda a um CPF dentro do arquivo de contratos, todos os dados em contratos serão printados
                        if (strcmp(cpf_consulta, contrato_consultado.id_consumidor) == 0) {
                            printf("\nID do Contrato: %s\n", contrato_consultado.id_contrato);
                            printf("ID do Consumidor: %s\n", contrato_consultado.id_consumidor);
                            printf("CNPJ da Miniusina: %s\n", contrato_consultado.cnpj_miniusina);
                            printf("Data do início do contrato: %s\n", contrato_consultado.data_inicio_contrato);
                            printf("Energia contratada: %s\n", contrato_consultado.energia);
                            contrato_encontrado = 1;
                        }
                    }
                }
                } if(consumidor_encontrado == 0) {
                    printf("\nPor favor, verifique o CPF cadastrado e tente novamente.\n");
                }
                if (contrato_encontrado == 0) {
                    printf("\nContrato não encontrado para o CPF digitado.\n");
                }
    }
    //Se a opção for o CNPJ...
    else if(strcmp(opcao_cnpj_cpf, "CNPJ") == 0 || strcmp(opcao_cnpj_cpf, "cnpj") == 0) {
        //Agora pedir para digitar o CNPJ sabendo que foi a opção escolhida
        printf("Digite o CNPJ: ");
        scanf("%s", cnpj_consulta);
        consumidores = fopen("consumidores.csv", "r");
        char primeira_linha[1000];
        fscanf(consumidores, "%[^\n]\n", primeira_linha);
        while (!feof(consumidores)) {
            fscanf(consumidores, "%[^;];%[^;];%[^\n]\n",
            consumidores_cadastrados.cnpj_consumidor,
            consumidores_cadastrados.nome_consumidor,
            consumidores_cadastrados.ra_consumidor);
            //Caso o cnpj digitado corresponda a um CNPJ dentro do arquivo de consumidores, todos os dados em consumidores serão printados
            if (strcmp(cnpj_consulta, consumidores_cadastrados.cnpj_consumidor) == 0) {
            printf("\nCNPJ: %s\n", consumidores_cadastrados.cnpj_consumidor);
            printf("Nome: %s\n", consumidores_cadastrados.nome_consumidor);
            printf("RA: %s\n", consumidores_cadastrados.ra_consumidor);
            consumidor_encontrado = 1;
            }
        } if (consumidor_encontrado == 1) {
                contratos = fopen("contratos.csv", "r");
                if (contratos != NULL) {
                    char primeira_linha[1000];
                    fscanf(contratos, "%[^\n]\n", primeira_linha);
                    while (!feof(contratos)) {
                        fscanf(contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                        contrato_consultado.id_contrato,
                        contrato_consultado.id_consumidor,
                        contrato_consultado.cnpj_miniusina,
                        contrato_consultado.data_inicio_contrato,
                        contrato_consultado.energia);
                        //Caso o cnpj digitado corresponda a um CNPJ dentro do arquivo de contratos, todos os dados de contratos serão printados
                        if (strcmp(cnpj_consulta, contrato_consultado.id_consumidor) == 0) {
                            printf("\nID do Contrato: %s\n", contrato_consultado.id_contrato);
                            printf("ID do Consumidor: %s\n", contrato_consultado.id_consumidor);
                            printf("CNPJ da Miniusina: %s\n", contrato_consultado.cnpj_miniusina);
                            printf("Data do início do contrato: %s\n", contrato_consultado.data_inicio_contrato);
                            printf("Energia contratada: %s\n", contrato_consultado.energia);
                            contrato_encontrado = 1;
                        }
                    }
                }
        }
                    if (contrato_encontrado == 0) {
                        printf("\nContrato não encontrado para o CNPJ digitado.\n");
                    }
                    if(consumidor_encontrado == 0) {
                        printf("\nPor favor, verifique o CNPJ cadastrado e tente novamente.\n");
                    } else {
                        printf("\nPor favor, verificar se a palavra CPF ou CNPJ foi digitada corretamente.\n");
                    }
    fclose(consumidores);
    fclose(contratos);
    }
}

//void para o item número 3 do menu - listar as miniusinas cadastradas
void listar_miniusinas() {
    FILE *miniusinas;
    struct miniusinas miniusina_cadastrada;
    //Vamos abrir o arquivo:
    miniusinas = fopen("miniusinas.csv", "r");
    //Depois, verificar se é possível abrir o arquivo:
    if (miniusinas != NULL) {
        char primeira_linha[1000];
        fscanf(miniusinas, "%[^\n]\n", primeira_linha);
        while (!feof(miniusinas)) {
            fscanf(miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
            miniusina_cadastrada.cnpj_miniusina,
            miniusina_cadastrada.nome_miniusina,
            miniusina_cadastrada.capacidade_total_miniusina,
            miniusina_cadastrada.ra_miniusina,
            miniusina_cadastrada.status_miniusina);
            //Agora, mostrando na tela/terminal
            printf("\nCNPJ: %s\n", miniusina_cadastrada.cnpj_miniusina);
            printf("Nome: %s\n", miniusina_cadastrada.nome_miniusina);
            printf("Capacidade Total: %s\n", miniusina_cadastrada.capacidade_total_miniusina);
            printf("RA: %s\n", miniusina_cadastrada.ra_miniusina);
            printf("Status: %s\n", miniusina_cadastrada.status_miniusina);
        }
    fclose(miniusinas);
    }
}

//Structs para o item número 4 do menu - listar as miniusinas por ordem decrescente de capacidade de geração de energia

//Struct 1 - Comparação de Capacidade para a ordenação em ordem decrescente
int comparar_capacidade(const void *y, const void *z) {
    //Para comparar as structs, vou criar uns ponteiros que vão representar "dados" diferentes, que serão comparados em breve
    struct miniusinas *miniusina1 = (struct miniusinas*)y;
    struct miniusinas *miniusina2 = (struct miniusinas*)z;

    //Convertendo de string para float, dado que a capacidade é um número com decimais
    float capacidade1 = atof(miniusina1->capacidade_total_miniusina);
    float capacidade2 = atof(miniusina2->capacidade_total_miniusina);

    //Aqui eu farei a manipulação dos índices - para que quando o número for menor, ele fique jogado "para trás/baixo".
    if (capacidade2 < capacidade1) {
        return -1;
    } else if (capacidade2 > capacidade1) {
        return 1;
    } else {
        return 0;
    }
}

//Struct 2 - Listagem das usinas em capacidade decrescente
void listar_miniusinas_capacidade_decrescente() {
    FILE *miniusinas; //representa o arquivo
    struct miniusinas miniusina_cadastrada;
    struct miniusinas listagem_miniusinas[TAM_MAX_QUANT_MINIUSINAS];
    int i, quantidade_miniusinas = 0;
    //Vamos abrir o arquivo:
    miniusinas = fopen("miniusinas.csv", "r");
    if (miniusinas != NULL) {
        char primeira_linha[1000];
        fscanf(miniusinas, "%[^\n]\n", primeira_linha);
        while (!feof(miniusinas)) {
            fscanf(miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
            miniusina_cadastrada.cnpj_miniusina,
            miniusina_cadastrada.nome_miniusina,
            miniusina_cadastrada.capacidade_total_miniusina,
            miniusina_cadastrada.ra_miniusina,
            miniusina_cadastrada.status_miniusina);
            listagem_miniusinas[quantidade_miniusinas] = miniusina_cadastrada;
            quantidade_miniusinas ++;
        }
        if (quantidade_miniusinas == 0) {
            printf("\nNenhuma miniusina registrada.");
        }
        else {
            //Hora de ordenar a lista com o quick sort
            qsort(listagem_miniusinas, quantidade_miniusinas, sizeof(struct miniusinas), comparar_capacidade);

            //Mostrar os dados no terminal com a ordem proposta
            for (int i = 0; i < quantidade_miniusinas; i++) {
                printf("\nCNPJ: %s\n", listagem_miniusinas[i].cnpj_miniusina);
                printf("Nome: %s\n", listagem_miniusinas[i].nome_miniusina);
                printf("Capacidade Total: %s\n", listagem_miniusinas[i].capacidade_total_miniusina);
                printf("RA: %s\n", listagem_miniusinas[i].ra_miniusina);
                printf("Status: %s\n", listagem_miniusinas[i].status_miniusina);
                }
        }
    } if (miniusinas == NULL) {
    printf("\nEste arquivo não conseguiu ser aberto.\n");
    }
    fclose(miniusinas);
}

//5 - Listar RAs por ordem decrescente de quantidade de miniusinas

//Struct de comparação da quantidade de miniusinas entre cada RA
int comparar_quantidade_ra(const void *y, const void *z) {
    struct ra *ra1 = (struct ra*)y;
    struct ra *ra2 = (struct ra*)z;

    // Hora de se acessar o valor contagem_miniusinas, contido dentro da struct de ra
    int contagem_ra1 = ra1->contagem_miniusinas;
    int contagem_ra2 = ra2->contagem_miniusinas;

    // Comparando a contagem em cada RA para se ordenar em ordem decrescente
    if (contagem_ra1 > contagem_ra2) {
        return -1;
    } else if (contagem_ra1 < contagem_ra2) {
        return 1;
    } else {
        return 0;
    }
}

//Struct para o item 5 do menu
void listar_ra_decrescente_miniusinas() {
    FILE *ra;
    FILE *miniusinas;
    struct ra ra_cadastrado;
    struct ra listagem_ras[TAM_MAX_QUANT_RAS];
    struct miniusinas miniusina_cadastrada;
    struct miniusinas listagem_miniusinas[TAM_MAX_QUANT_MINIUSINAS];
    int i, quantidade_miniusinas = 0, quantidade_ras = 0;
    //Hora de abrir os arquivos necessários
    ra = fopen("ra.csv", "r");
    if (ra != NULL) {
        char primeira_linha[1000];
        fscanf(ra, "%[^\n]\n", primeira_linha);
        while (!feof(ra)) {
            fscanf(ra, "%[^;];%[^\n]\n",
            ra_cadastrado.id_ra,
            ra_cadastrado.nome_ra);
            listagem_ras[quantidade_ras] = ra_cadastrado;
            quantidade_ras ++;
            //Vamos precisar estar com o arquivo de ras aberto, porque alguns dados que vamos precisar lá na frente dependem deles
            miniusinas = fopen("miniusinas.csv", "r");
            if (miniusinas != NULL) {
                char primeira_linha[1000];
                fscanf(miniusinas, "%[^\n]\n", primeira_linha);
                while (!feof(miniusinas)) {
                    fscanf(miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                    miniusina_cadastrada.cnpj_miniusina,
                    miniusina_cadastrada.nome_miniusina,
                    miniusina_cadastrada.capacidade_total_miniusina,
                    miniusina_cadastrada.ra_miniusina,
                    miniusina_cadastrada.status_miniusina);
                    //Aqui, se o valor ra, contido dentro de miniusina, corresponder a id, dentro do arquivo de ra, então é porque
                    //a RA em questão tem uma miniusina
                    if (strcmp(miniusina_cadastrada.ra_miniusina, ra_cadastrado.id_ra) == 0) {
                        listagem_miniusinas[quantidade_miniusinas] = miniusina_cadastrada;
                        quantidade_miniusinas ++;
                    }
                }
            }
        }
    }
        if (quantidade_miniusinas == 0) {
            printf("\nNenhuma miniusina registrada.\n");
        }
        if (quantidade_ras == 0) {
            printf("\nNenhuma RA registrada.\n");
        }
        else {
            //Hora de computar o número de miniusinas para cada RA e guardar na variável criada no struct das RAs, contagem_miniusinas
            for (int i = 0; i < quantidade_ras; i++) {
                listagem_ras[i].contagem_miniusinas = 0;
                for (int j = 0; j < quantidade_miniusinas; j++) {
                    //Aqui, haverá uma iteração que colocará o valor certo de miniusinas para cada RA, guardando, pois, este valor.
                    if (strcmp(listagem_miniusinas[j].ra_miniusina, listagem_ras[i].id_ra) == 0) {
                        listagem_ras[i].contagem_miniusinas++;
                    }
                }
            }
            //Hora de ordenar com base no struct de ordenagem anterior, comparar_quantidade_ra.
            qsort(listagem_ras, quantidade_ras, sizeof(struct ra), comparar_quantidade_ra);
            //Hora de mostrar os dados no terminal com a ordem proposta
            for (int i = 0; i < quantidade_ras; i++) {
                printf("\nRA número: %s\n", listagem_ras[i].id_ra);
                printf("Nome: %s\n", listagem_ras[i].nome_ra);
                //Aqui vamos criar uma variável de contagem para que o valor correto de cada RA possa ser printado
                int contagem_ra = 0;
                    for (int j = 0; j < quantidade_miniusinas; j++) {
                        //Agora vamos aumentar a contagem de RAs caso haja correspondencia entre o ID da RA no arquivo miniusinas
                        //e o ID no arquivo RA
                        if (strcmp(listagem_miniusinas[j].ra_miniusina, listagem_ras[i].id_ra) == 0) {
                            contagem_ra++;
                        }
                    }
                printf("Quantidade de Miniusinas: %d\n", contagem_ra);
            }
        }
        if (miniusinas == NULL) {
        printf("\nEste arquivo não foi encontrado.\n");
        }
    fclose(miniusinas);
    fclose(ra);
}

//6 - Listar RAs por ordem crescente de número de contratos

//Struct que vai colocar os contratos em ordem crescente
int comparar_quantidade_contratos(const void *y, const void *z) {
    struct ra *ra1 = (struct ra*)y;
    struct ra *ra2 = (struct ra*)z;

    // Comparando a contagem em cada RA para se ordenar em ordem decrescente
    if (ra1->contagem_contratos < ra2->contagem_contratos) {
        return -1;
    } else if (ra1->contagem_contratos > ra2->contagem_contratos) {
        return 1;
    } else {
        return 0;
    }
}

//Struct para o item 6 do menu
void listar_ra_crescente_contratos() {
    FILE *ra;
    FILE *miniusinas;
    FILE *contratos;
    struct ra ra_cadastrado;
    struct ra listagem_ras[TAM_MAX_QUANT_RAS];
    struct miniusinas miniusina_cadastrada;
    struct miniusinas listagem_miniusinas[TAM_MAX_QUANT_MINIUSINAS];
    struct contratos contrato_consultado;
    struct contratos listagem_contratos[TAM_MAX_QUANT_CONTRATOS];
    int i, quantidade_miniusinas = 0, quantidade_ras = 0, quantidade_contratos = 0, contrato_encontrado = 0;
    //Hora de abrir os arquivos necessários
    ra = fopen("ra.csv", "r");
    //Depois, verificar se consegue-se abrir o arquivo:
    if (ra != NULL) {
        char primeira_linha[1000];
        fscanf(ra, "%[^\n]\n", primeira_linha);
        while (!feof(ra)) {
            fscanf(ra, "%[^;];%[^\n]\n",
            ra_cadastrado.id_ra,
            ra_cadastrado.nome_ra);
            listagem_ras[quantidade_ras] = ra_cadastrado;
            quantidade_ras ++;
        }
            //Agora, abrir o arquivo de miniusinas, porque por meio dele poderemos saber a qual CNPJ (informação contida em contratos) está ligado a cada RA (o que não está em contratos)
            miniusinas = fopen("miniusinas.csv", "r");
            if (miniusinas != NULL) {
                char primeira_linha[1000];
                fscanf(miniusinas, "%[^\n]\n", primeira_linha);
                while (!feof(miniusinas)) {
                    fscanf(miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                    miniusina_cadastrada.cnpj_miniusina,
                    miniusina_cadastrada.nome_miniusina,
                    miniusina_cadastrada.capacidade_total_miniusina,
                    miniusina_cadastrada.ra_miniusina,
                    miniusina_cadastrada.status_miniusina);
                    listagem_miniusinas[quantidade_miniusinas] = miniusina_cadastrada;
                    quantidade_miniusinas ++;
            }
                }
            //Agora, abriremos o arquivo de contratos e faremos um loop com ele
                contratos = fopen("contratos.csv", "r");
                if (contratos != NULL) {
                    char primeira_linha[1000];
                    fscanf(contratos, "%[^\n]\n", primeira_linha);
                    while (!feof(contratos)) {
                        fscanf(contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                        contrato_consultado.id_contrato,
                        contrato_consultado.id_consumidor,
                        contrato_consultado.cnpj_miniusina,
                        contrato_consultado.data_inicio_contrato,
                        contrato_consultado.energia);
                        listagem_contratos[quantidade_contratos] = contrato_consultado;
                        quantidade_contratos++;
                    }
                }
                if (quantidade_contratos == 0 || quantidade_miniusinas == 0 || quantidade_ras == 0) {
                    printf("\nHouve problema na leitura de um dos arquivos\n");
                } else {
                    //Computar o número de contratos para cada RA e guardar na variável adequada
                    for (int i = 0; i < quantidade_ras; i++) {
                        listagem_ras[i].contagem_contratos = 0;
                        for (int j = 0; j < quantidade_miniusinas; j++) {
                            for (int k = 0; k < quantidade_contratos+1; k++) {
                                //Se os dados da RA e CNPJ forem correspondente entre as miniusinas e RAs
                                if (strcmp(listagem_miniusinas[k].ra_miniusina, listagem_ras[i].id_ra) == 0 &&
                                        strcmp(listagem_contratos[j].cnpj_miniusina, listagem_miniusinas[k].cnpj_miniusina) == 0)  {
                                        listagem_ras[i].contagem_contratos++;
                                    }
                            }
                        }
                    }
                    // Colocar RAs em ordem crescente do número de contratos
                    qsort(listagem_ras, quantidade_ras, sizeof(struct ra), comparar_quantidade_contratos);

                    // Mostrar os dados no terminal de maneira adequada
                    for (int i = 0; i < quantidade_ras; i++) {
                        printf("\nRA número: %s\n", listagem_ras[i].id_ra);
                        printf("Nome: %s\n", listagem_ras[i].nome_ra);
                        int contagem_contratos = 0;
                            for (int j = 0; j < quantidade_miniusinas; j++) {
                                for (int k = 0; k < quantidade_contratos; k++) {
                                    //Se os dados da RA e CNPJ forem compatíveis com a listagem de contratos, é porque o contrato existe
                                    if (strcmp(listagem_miniusinas[k].ra_miniusina, listagem_ras[i].id_ra) == 0 &&
                                    strcmp(listagem_contratos[j].cnpj_miniusina, listagem_miniusinas[k].cnpj_miniusina) == 0)  {
                                    contagem_contratos++;
                                }
                            }
                        }
                        printf("Quantidade de contratos: %d\n", listagem_ras[i].contagem_contratos);
                    }
                }
    }
            fclose(miniusinas);
            fclose(ra);
            fclose(contratos);
}

//7 - Listar as RAs por ordem decrescente de capacidade de geração

//Aproveitando o struct do item 4 para colocar a capacidade de geração em ordem decrescente
int comparar_capacidade_ra(const void *y, const void *z) {
    struct ra *ra1 = (struct ra*)y;
    struct ra *ra2 = (struct ra*)z;
    //Eu criei um float chamado capacidade total dentro da struct RA para não ter de lidar com a comparação entre mais de 1 struct
    if (ra2->capacidade_total < ra1->capacidade_total) {
        return -1;
    } else if (ra2->capacidade_total > ra1->capacidade_total) {
        return 1;
    } else {
        return 0;
    }
}

//Struct com o que deve ser realizado no item 7 em si
void listar_ra_decrescente_capacidade() {
    FILE *ra;
    FILE *miniusinas;
    struct ra ra_cadastrado;
    struct ra listagem_ras[TAM_MAX_QUANT_RAS];
    struct miniusinas miniusina_cadastrada;
    struct miniusinas listagem_miniusinas[TAM_MAX_QUANT_MINIUSINAS];
    int i, quantidade_miniusinas = 0, quantidade_ras = 0;
    //Hora de abrir os arquivos necessários
    ra = fopen("ra.csv", "r");
    if (ra != NULL) {
        char primeira_linha[1000];
        fscanf(ra, "%[^\n]\n", primeira_linha);
        while (!feof(ra)) {
            fscanf(ra, "%[^;];%[^\n]\n",
            ra_cadastrado.id_ra,
            ra_cadastrado.nome_ra);
            listagem_ras[quantidade_ras] = ra_cadastrado;
            quantidade_ras ++;
        //Hora de abrir o arquivo das miniusinas para verificarmos os RAs delas
        miniusinas = fopen("miniusinas.csv", "r");
        if (miniusinas != NULL) {
            char primeira_linha[1000];
            fscanf(miniusinas, "%[^\n]\n", primeira_linha);
            while (!feof(miniusinas)) {
                fscanf(miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                miniusina_cadastrada.cnpj_miniusina,
                miniusina_cadastrada.nome_miniusina,
                miniusina_cadastrada.capacidade_total_miniusina,
                miniusina_cadastrada.ra_miniusina,
                miniusina_cadastrada.status_miniusina);
                //Isso é para a contagem de miniusinas que usarei mais tarde no laço de for
                if (strcmp(miniusina_cadastrada.ra_miniusina, ra_cadastrado.id_ra) == 0) {
                    listagem_miniusinas[quantidade_miniusinas] = miniusina_cadastrada;
                    quantidade_miniusinas ++;
                }
            }
        }
        }
    }
        if (quantidade_miniusinas == 0 || quantidade_ras == 0) {
            printf("\nHouve problema na leitura de um dos arquivos.\n");
            }
        else {
        // Calcular a capacidade total de cada RA e guardar na variável capacidade_total, dentro da struct ra
        for (int i = 0; i < quantidade_ras; i++) {
            float capacidade_total = 0.0;
            for (int j = 0; j < quantidade_miniusinas; j++) {
                if (strcmp(listagem_miniusinas[j].ra_miniusina, listagem_ras[i].id_ra) == 0) {
                    capacidade_total += atof(listagem_miniusinas[j].capacidade_total_miniusina);
                }
            }
            listagem_ras[i].capacidade_total = capacidade_total;
        }
        //Ordenar a lista
        qsort(listagem_ras, quantidade_ras, sizeof(struct ra), comparar_capacidade_ra);
        //Mostrar os dados no terminal com a ordem proposta
        for (int i = 0; i < quantidade_ras; i++) {
            printf("\nRA número: %s\n", listagem_ras[i].id_ra);
            printf("Nome: %s\n", listagem_ras[i].nome_ra);
            printf("Capacidade de geração: %.2f\n", listagem_ras[i].capacidade_total);
        }
        }
}

//8 - Listar as RAs por ordem decrescente de percentual de energia disponível

//Struct de ordenação
int comparar_percentual_contratado(const void *y, const void *z) {
    struct ra *ra1 = (struct ra*)y;
    struct ra *ra2 = (struct ra*)z;

    //Aqui vamos lidar com RA's sem e com capacidade total (porque às vezes elas não tem energia contratada, então o percentual fica zero)
    if (ra1->capacidade_total == 0 && ra2->capacidade_total != 0) {
        return 1;
    } else if (ra1->capacidade_total != 0 && ra2->capacidade_total == 0) {
        return -1;
    }

    //Aqui vamos lidar com RA's com percentual maior do que a outra
    if (ra2->percentual_contratado > ra1->percentual_contratado) {
        return -1;
    } else if (ra2->percentual_contratado < ra1->percentual_contratado) {
        return 1;
    } else {
        return 0;
    }
}

//Void para o item 8 em si
void listar_ra_decrescente_percentual() {
    FILE *ra;
    FILE *miniusinas;
    FILE *contratos;
    struct ra ra_cadastrado;
    struct ra listagem_ras[TAM_MAX_QUANT_RAS];
    struct miniusinas miniusina_cadastrada;
    struct miniusinas listagem_miniusinas[TAM_MAX_QUANT_MINIUSINAS];
    struct contratos contrato_consultado;
    struct contratos listagem_contratos[TAM_MAX_QUANT_CONTRATOS];
    int i, quantidade_miniusinas = 0, quantidade_ras = 0, quantidade_contratos = 0, contrato_encontrado = 0;
    //Hora de abrir os arquivos necessários
    ra = fopen("ra.csv", "r");
    if (ra != NULL) {
        char primeira_linha[1000];
        fscanf(ra, "%[^\n]\n", primeira_linha);
        while (!feof(ra)) {
            fscanf(ra, "%[^;];%[^\n]\n",
            ra_cadastrado.id_ra,
            ra_cadastrado.nome_ra);
            listagem_ras[quantidade_ras] = ra_cadastrado;
            quantidade_ras ++;
        }
    }
      miniusinas = fopen("miniusinas.csv", "r");
      if (miniusinas != NULL) {
            char primeira_linha[1000];
            fscanf(ra, "%[^\n]\n", primeira_linha);
              while (!feof(miniusinas)) {
                  fscanf(miniusinas, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                  miniusina_cadastrada.cnpj_miniusina,
                  miniusina_cadastrada.nome_miniusina,
                  miniusina_cadastrada.capacidade_total_miniusina,
                  miniusina_cadastrada.ra_miniusina,
                  miniusina_cadastrada.status_miniusina);
                  listagem_miniusinas[quantidade_miniusinas] = miniusina_cadastrada;
                  quantidade_miniusinas ++;
                  }
        }
        contratos = fopen("contratos.csv", "r");
        if (contratos != NULL) {
            char primeira_linha[1000];
            fscanf(contratos, "%[^\n]\n", primeira_linha);
            while (!feof(contratos)) {
                fscanf(contratos, "%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
                contrato_consultado.id_contrato,
                contrato_consultado.id_consumidor,
                contrato_consultado.cnpj_miniusina,
                contrato_consultado.data_inicio_contrato,
                contrato_consultado.energia);
                listagem_contratos[quantidade_contratos] = contrato_consultado;
                quantidade_contratos++;
            }
        }
        if (quantidade_miniusinas == 0 || quantidade_ras == 0 || quantidade_contratos == 0) {
            printf("\nHouve problema na leitura de um dos arquivos.\n");}
        else {
        //Vou aproveitar o loop do item anterior para guardar os valores da capacidade de geração na struct ra
        for (int i = 0; i < quantidade_ras; i++) {
            float capacidade_total = 0.0;
            for (int j = 0; j < quantidade_miniusinas; j++) {
                if (strcmp(listagem_miniusinas[j].ra_miniusina, listagem_ras[i].id_ra) == 0) {
                    capacidade_total += atof(listagem_miniusinas[j].capacidade_total_miniusina);
                }
            }
            listagem_ras[i].capacidade_total = capacidade_total;
        }
        //Agora, um loop similar para que os valores de energia contratada sejam guardados na struct ra
        for (int i = 0; i < quantidade_ras; i++) {
                float energia_contratada = 0.0;
                for (int j = 0; j < quantidade_miniusinas; j++) {
                    for (int k = 0; k < quantidade_contratos; k++) {
                        if (strcmp(listagem_miniusinas[j].ra_miniusina, listagem_ras[i].id_ra) == 0 &&
                                strcmp(listagem_contratos[k].cnpj_miniusina, listagem_miniusinas[j].cnpj_miniusina) == 0)  {
                                energia_contratada += atof(listagem_contratos[k].energia);
                                }
                        }
                    }
                    listagem_ras[i].energia_contratada = energia_contratada;
        }
        for (int i = 0; i < quantidade_ras; i++) {
            if (listagem_ras[i].capacidade_total != 0) {
                listagem_ras[i].percentual_contratado = listagem_ras[i].energia_contratada/listagem_ras[i].capacidade_total;
            } else {
                listagem_ras[i].percentual_contratado = 0;
            }
        }
        //Hora de fazer o sort
        qsort(listagem_ras, quantidade_ras, sizeof(struct ra), comparar_percentual_contratado);
        //Mostrar os dados no terminal de maneira adequada
        for (int i = 0; i < quantidade_ras; i++) {
                printf("\nRA número: %s\n", listagem_ras[i].id_ra);
                printf("Nome: %s\n", listagem_ras[i].nome_ra);
                if (listagem_ras[i].capacidade_total != 0) { //Isso evita com que uma RA sem energia e sem capacidade gere divisão 0 por 0
                    printf("Percentual de energia disponível: %.2f%%\n", 100 - 100 * (listagem_ras[i].energia_contratada/listagem_ras[i].capacidade_total));
                } else {
                    printf("Percentual de energia disponível: 0,00%%\n");
                }
                listagem_ras[i].percentual_contratado = listagem_ras[i].energia_contratada/listagem_ras[i].capacidade_total;
        }
        }
}

int main() {
    setlocale(LC_ALL,"Portuguese"); //Para que o terminal mostre acentos e caracteres de português
    int numero_escolhido;
    int resultado;
    //Menu de opções - Usar uma estrutura de repetição e deixar sair somente quando o número 9 for digitado
printf("*** Este é o Menu de Opções do SINGDF - Geração e Distribuição Fotovoltaica. ***\n");
printf("*** Por aqui, você pode registrar e acompanhar as miniusinas e consumidores do SINGDF! ***\n");
//Inserir uma estrutura de repetição com as opções aqui (guardar o número numa variável)
    do {
        menu();
        resultado = scanf("%d", &numero_escolhido); //Isso serve para verificar se aquilo que foi digitado é um int ou não
        if (resultado == 0) {
            //Se resultado for igual a zero, o valor digitado não foi um número inteiro e limparemos o buffer.
            numero_escolhido = 0;
            fflush(stdin);
        }
        if (numero_escolhido == 1) {
            consultar_miniusina();
        } else if (numero_escolhido == 2) {
                consultar_consumidor();
        } else if (numero_escolhido == 3) {
                listar_miniusinas();
        } else if (numero_escolhido == 4) {
                listar_miniusinas_capacidade_decrescente();
        } else if (numero_escolhido == 5) {
                listar_ra_decrescente_miniusinas();
        } else if (numero_escolhido == 6) {
                listar_ra_crescente_contratos();
        } else if (numero_escolhido == 7) {
                listar_ra_decrescente_capacidade();
        } else if (numero_escolhido == 8) {
                listar_ra_decrescente_percentual();
        } else {
                printf("\nPor favor, digite um número entre 1 e 9 correspondente à função desejada!\n");
        }} while(numero_escolhido != 9);
    return 0;
    }
