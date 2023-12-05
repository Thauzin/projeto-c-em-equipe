#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

typedef struct {
    char placa[8];
    char modelo[20];
    char marca[20];
    char cor[20];
} Veiculo;

typedef struct {
    int numVaga;
    char placaVeiculo[8];
    char dataChegada[11];
    char horaChegada[6];
    int numHoras;
} Ocupacao;

void limparTela() {
    system("@cls||clear");
}

int placaExisteNoArquivo(const char *placa) {
    FILE *arquivo = fopen("veiculos.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de veículos.\n");
        return 1; 
    }

    Veiculo veiculoArquivo;
    while (fscanf(arquivo, "%s %s %s %s",
                  veiculoArquivo.placa,
                  veiculoArquivo.modelo,
                  veiculoArquivo.marca,
                  veiculoArquivo.cor) == 4) {
        if (strcasecmp(veiculoArquivo.placa, placa) == 0) {
            fclose(arquivo);

            printf("Veículo com a mesma placa já cadastrado. Operação cancelada.\n");
            printf("Detalhes do veículo existente:\n");
            printf("Placa: %s\n", veiculoArquivo.placa);
            printf("Modelo: %s\n", veiculoArquivo.modelo);
            printf("Marca: %s\n", veiculoArquivo.marca);
            printf("Cor: %s\n", veiculoArquivo.cor);

            return 1; 
        }
    }

    fclose(arquivo);
    return 0; 
}

void cadastrarVeiculo(Veiculo veiculos[], int *numVeiculos) {
    if (*numVeiculos < 50) {
        int placaValida = 0;

        while (!placaValida) {
            printf("Digite a placa do veículo (7 dígitos): ");
            scanf("%s", veiculos[*numVeiculos].placa);

            if (strlen(veiculos[*numVeiculos].placa) == 7) {
                placaValida = 1;
            } else {
                printf("Placa inválida! A placa deve conter exatamente 7 dígitos.\n");
            }
        }

        if (placaExisteNoArquivo(veiculos[*numVeiculos].placa)) {
            
            return;
        }

        printf("Digite o modelo do veículo: ");
        scanf("%s", veiculos[*numVeiculos].modelo);

        printf("Digite a marca do veículo: ");
        scanf("%s", veiculos[*numVeiculos].marca);

        printf("Digite a cor do veículo: ");
        scanf("%s", veiculos[*numVeiculos].cor);

        FILE *arquivo = fopen("veiculos.txt", "a");

        if (arquivo != NULL) {
            fprintf(arquivo, "%s %s %s %s\n",
                    veiculos[*numVeiculos].placa,
                    veiculos[*numVeiculos].modelo,
                    veiculos[*numVeiculos].marca,
                    veiculos[*numVeiculos].cor);

            fclose(arquivo);

            (*numVeiculos)++;
            printf("Veículo cadastrado com sucesso e salvo no arquivo!\n");
        } else {
            printf("Erro ao abrir o arquivo para escrita.\n");
        }
    } else {
        printf("Limite de veículos cadastrados atingido!\n");
    }
}

int ocupadas() {
    system("cls");
    FILE *arquivo = fopen("vagas.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char linha[100];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }

    fclose(arquivo);
    return 0;
}

int buscarVeiculoPorPlaca(Veiculo veiculos[], int numVeiculos, const char *placa) {
    for (int i = 0; i < numVeiculos; i++) {
        if (strcmp(veiculos[i].placa, placa) == 0) {
            return i;
        }
    }

    FILE *arquivo = fopen("veiculos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de veículos.\n");
        return -1;
    }

    Veiculo veiculoArquivo;
    while (fscanf(arquivo, "%s %s %s %s",
                  veiculoArquivo.placa,
                  veiculoArquivo.modelo,
                  veiculoArquivo.marca,
                  veiculoArquivo.cor) == 4) {
        if (strcmp(veiculoArquivo.placa, placa) == 0) {
            fclose(arquivo);
            return numVeiculos;
        }
        numVeiculos++;
    }

    fclose(arquivo);
    return -1;
}

void listarVagasLivres(Ocupacao ocupacoes[], int numOcupacoes, int numVagas) {
    int vagasLivres[numVagas];
    int numVagasOcupadas = 0;

    for (int i = 0; i < numVagas; i++) {
        vagasLivres[i] = 1;
    }

    for (int i = 0; i < numOcupacoes; i++) {
        if (ocupacoes[i].numVaga != 0) {
            vagasLivres[ocupacoes[i].numVaga - 1] = 0;
            numVagasOcupadas++;
        }
    }

    printf("Vagas livres: ");
    for (int i = 0; i < numVagas; i++) {
        if (vagasLivres[i]) {
            printf("%d ", i + 1);
        }
    }

    if (numVagasOcupadas == numVagas) {
        printf("\nTodas as vagas estão ocupadas.\n");
    } else {
        printf("\n");
    }
}


void ocuparVaga(Ocupacao ocupacoes[], int *numOcupacoes, Veiculo veiculos[], int numVeiculos, int numVagas) {
    listarVagasLivres(ocupacoes, *numOcupacoes, numVagas);

    if (*numOcupacoes < numVagas) {
        Ocupacao ocupacao;
        int indiceVeiculo;

        printf("Digite a placa do veículo: ");
        scanf("%s", ocupacao.placaVeiculo);

        for (int i = 0; ocupacao.placaVeiculo[i]; i++) {
            ocupacao.placaVeiculo[i] = toupper(ocupacao.placaVeiculo[i]);
        }

        
        for (int i = 0; i < *numOcupacoes; i++) {
            for (int j = 0; ocupacoes[i].placaVeiculo[j]; j++) {
                ocupacoes[i].placaVeiculo[j] = toupper(ocupacoes[i].placaVeiculo[j]);
            }

            if (strcmp(ocupacoes[i].placaVeiculo, ocupacao.placaVeiculo) == 0) {
                printf("Veículo já está estacionado na vaga %d.\n", ocupacoes[i].numVaga);
                printf("Data e Hora de Chegada: %s %s:%s\n", ocupacoes[i].dataChegada, ocupacoes[i].horaChegada, ocupacoes[i].horaChegada + 3);
                return;
            }
        }

        indiceVeiculo = buscarVeiculoPorPlaca(veiculos, numVeiculos, ocupacao.placaVeiculo);

        if (indiceVeiculo != -1) {
            printf("Digite o número da vaga desejada: ");
            scanf("%d", &ocupacao.numVaga);

            if (ocupacao.numVaga < 1 || ocupacao.numVaga > numVagas) {
                printf("Número de vaga inválido. Operação cancelada.\n");
                return;
            }

            for (int i = 0; i < *numOcupacoes; i++) {
                if (ocupacoes[i].numVaga == ocupacao.numVaga) {
                    printf("Vaga já ocupada. Operação cancelada.\n");
                    return;
                }
            }

            printf("Digite a data de chegada (dd/mm/aa): ");
            scanf("%s", ocupacao.dataChegada);

            printf("Digite a hora de chegada (HH): ");
            scanf("%s", ocupacao.horaChegada);

            printf("Digite os minutos de chegada (MM): ");
            scanf("%s", ocupacao.horaChegada + 3);

            ocupacoes[*numOcupacoes] = ocupacao;
            (*numOcupacoes)++;

            int minutos = atoi(ocupacao.horaChegada + 3);
            sprintf(ocupacao.horaChegada + 3, "%02d", minutos);

            FILE *documento = fopen("vagas.txt", "a");
            if (documento != NULL) {
                fprintf(documento, "Placa: %s, Vaga: %d, Data e Hora de Chegada: %s %s:%s\n",
                        ocupacao.placaVeiculo,
                        ocupacao.numVaga,
                        ocupacao.dataChegada,
                        ocupacao.horaChegada,
                        ocupacao.horaChegada + 3);
                fclose(documento);
                printf("Vaga ocupada com sucesso!\n");
            } else {
                printf("Erro ao criar o documento de documentação.\n");
            }
        } else {
            printf("Veículo não encontrado. Operação cancelada.\n");
        }
    } else {
        printf("Todas as vagas estão ocupadas. Impossível ocupar mais vagas.\n");
    }
}

float calcularValorTotal(float valorHora, int numHoras) {
    return numHoras * valorHora;
}

void realizarPagamento(Ocupacao ocupacoes[], int numOcupacoes, float valorHora) {
    char placa[8];
    printf("Digite a placa do veículo: ");
    scanf("%s", placa);

    int indiceOcupacao = -1;


    for (int i = 0; i < numOcupacoes; i++) {
        if (strcmp(ocupacoes[i].placaVeiculo, placa) == 0) {
            indiceOcupacao = i;
            break;
        }
    }

    if (indiceOcupacao != -1) {
        int numHorasEstacionado;
        printf("Digite a quantidade de horas que o veículo ficou estacionado: ");
        scanf("%d", &numHorasEstacionado);

        float valorTotal = calcularValorTotal(valorHora, numHorasEstacionado);

        printf("Placa do veículo: %s\n", ocupacoes[indiceOcupacao].placaVeiculo);
        printf("Tempo estacionado: %d horas\n", numHorasEstacionado);
        printf("Valor total a ser pago: R$%.2f\n", valorTotal);

        int opcaoPagamento;
        printf("Escolha o método de pagamento:\n");
        printf("1. Cartão\n");
        printf("2. Dinheiro\n");
        printf("3. Pix\n");
        printf("4. Fiado\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcaoPagamento);

        switch (opcaoPagamento) {
            case 1:
                printf("Pagamento com cartão realizado com sucesso!\n");
                break;
            case 2:
                printf("Pagamento em dinheiro realizado com sucesso!\n");
                break;
            case 3:
                printf("Pagamento via Pix realizado com sucesso!\n");
                break;
            case 4:
                printf("Pagamento a fiado registrado com sucesso!\n");
                break;
            default:
                printf("Opção de pagamento inválida!\n");
                break;
        }
    } else {
        printf("Veículo não encontrado. O pagamento não pode ser processado.\n");
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    float valorHora = 0; 
    int valorHoraCadastrado = 0;  
    int numVagas = 0;
    int numVeiculos = 0;
    int vagasOcupadas = 0;
    Veiculo veiculos[50];
    Ocupacao ocupacoes[50];

    do {
        printf("Bem-vindo ao Estacionamento dos Brother\n");

        if (valorHoraCadastrado) {
            printf("Valor por hora atual: %.2f\n", valorHora);
        }

        printf("1. Cadastro do valor por hora\n");
        printf("2. Cadastro de vagas\n");
        printf("3. Cadastro do veículo\n");
        printf("4. Ocupar uma vaga\n");
        printf("5. Listar as vagas livres\n");
        printf("6. Listar as vagas ocupadas\n");
        printf("7. Pagamento\n");
        printf("8. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor por hora: ");
                scanf("%f", &valorHora);
                valorHoraCadastrado = 1;  
                printf("Cadastro de valor por hora efetuado com sucesso! \n");
                break;

            case 2:
                printf("Digite o numero de vagas: ");
                scanf("%d", &numVagas);
                printf("Cadastro de numero de vagas o efetuado com sucesso! \n");
                break;

            case 3:
                cadastrarVeiculo(veiculos, &numVeiculos);
                break;

            case 4:
                ocuparVaga(ocupacoes, &vagasOcupadas, veiculos, numVeiculos, numVagas);
                break;

            case 5:
                listarVagasLivres(ocupacoes, vagasOcupadas, numVagas);
                break;

            case 6:
                ocupadas();
                break;

            case 7:
                realizarPagamento(ocupacoes, vagasOcupadas, valorHora);
                break;

            case 8:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opção inválida!\n");
                break;
        }

        printf("Pressione enter para continuar...\n");
        getchar();
        getchar();
        limparTela();

    } while (opcao != 8);

    return 0;
}