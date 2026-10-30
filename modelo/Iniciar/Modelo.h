// Garante que este arquivo seja incluído só uma vez por compilação
#pragma once
// Importa as bibliotecas
#include <Arduino.h>
#include <math.h>
#include <vector>

// Inclui os arquivos do projeto: dataset, prints, leitura e cálculos
#include "Dataset.h"
#include "Prints.h"
#include "Leitura.h"
#include "Calculos.h"

// Define o caminho do arquivo CSV contendo os dados do dataset
const char* caminho = "/Bateria.csv";

// Gera um número float aleatório entre 0 e 1 usando o gerador de hardware do ESP32
float gerarMumeroAleatorio() {
  return (float)((double)esp_random() / 4294967295.0);
}

// Solicita um número pelo Monitor Serial e devolve o valor como float
float lerNumero(const char* mensagem) {
  // Exibe a mensagem de solicitação no Monitor Serial
  Serial.print(mensagem);
  // Inicia um loop para limpar o buffer de leitura da Serial
  while (Serial.available()) 
  // Lê e descarta os caracteres pendentes no buffer
  Serial.read();
  // Aguarda até que algum dado seja digitado e recebido pela Serial
  while (!Serial.available())
  // Aplica um pequeno atraso para não sobrecarregar o processador durante a espera
  delay(10);
  // Lê a string digitada pelo usuário até encontrar a quebra de linha
  String textoDigitado = Serial.readStringUntil('\n');
  // Remove espaços em branco e quebras de linha residuais da string
  textoDigitado.trim();
  // Exibe o valor processado no Monitor Serial para confirmação
  Serial.println(textoDigitado);
  // Converte a string final para ponto flutuante (float) e a retorna
  return textoDigitado.toFloat();
}

// Executa uma consulta completa do modelo
void executarModelo() {
  // Cria o objeto "bateria" e lê o arquivo CSV contendo os dados do dataset e armazena na estrutura "bateria"
  Dataset bateria;
  if (!lerDataset(caminho, bateria)) {
    Serial.println("Erro: não foi possível ler o dataset.");
    // Interrompe a execução da função em caso de falha na leitura do arquivo
    return;
  }

  // Solicita ao usuário que insira o valor da carga e da distância
  float carga = lerNumero("Digite a carga (kg): ");
  // Solicita e lê o valor da distância percorrida inserido pelo usuário
  float distancia = lerNumero("Digite a distância (km): ");

  // Define os limites mínimo e máximo aceitáveis para o peso da carga (em kg)
  const float cargaMin = 1.0, cargaMax = 100;
  // Define os limites mínimo e máximo aceitáveis para a distância da viagem (em km)
  const float distanciaMin = 1.0, distanciaMax = 9.0;
  // Verifica se o valor da carga informada está fora dos limites físicos permitidos
  if (!(cargaMin <= carga && carga <= cargaMax)) {
    Serial.printf("Erro: Carga (%.2f kg) fora do limite físico permitido (%.1f a %.1f kg). Dado não salvo.\n", carga, cargaMin, cargaMax);
  }
  // Verifica se o valor da distância informada está fora dos limites de operação permitidos
  else if (!(distanciaMin <= distancia && distancia <= distanciaMax)) {
    Serial.printf("Erro: Distância (%.2f km) fora do limite permitido (%.1f a %.1f km). Dado não salvo.\n", distancia, distanciaMin, distanciaMax);
  }
  else { // Caso os dados inseridos sejam válidos, prossegue com a preparação e execução do modelo
    
    // Etapa 1 e 2: Preparo dos Dados
    const std::vector<float>& medicoes_carga = bateria.carga;
    const std::vector<float>& medicoes_distancia = bateria.distancia;
    const std::vector<float>& gasto_viagens = bateria.gasto;
    // Obtém a quantidade total de amostras presentes no dataset carregado
    const int numeroTotalAmostras = (int)gasto_viagens.size();

    // ==================================================================================================== //
    
    // Etapa 3: Inicialização dos Parâmetros (Pesos e Viés via Glorot)
    float pesos[2];
    float vies = 0.0f;
    // Calcula o limite para inicialização dos pesos usando o método de Glorot Uniforme (Xavier)
    const float limite_glorot = sqrtf(6.0f / (2 + 1));
    pesos[0] = (gerarMumeroAleatorio() * 2.0f - 1.0f) * limite_glorot;
    pesos[1] = (gerarMumeroAleatorio() * 2.0f - 1.0f) * limite_glorot;

    // ==================================================================================================== //

    // Etapa 4: Compilar o Modelo (Otimizador Adam do Calculos.h)

    OtimizadorAdam otimizadorAdam;
    otimizadorAdam.inicializar(0.05f, 0.9f, 0.999f, 1e-7f);

    // ==================================================================================================== //

    // Etapa 5: Treinamento do Modelo (500 Épocas)

    const int epochs = 500;
    // Define o tamanho do lote (mini-batch) para a atualização dos gradientes
    const int tamanho_lote = 32;
    // Cria um vetor para armazenar os índices das amostras a fim de permitir o embaralhamento
    std::vector<int> indices(numeroTotalAmostras);

    // Preenche o vetor de índices de forma sequencial com base no número total de amostras
    for (int indiceAmostra = 0; indiceAmostra < numeroTotalAmostras; indiceAmostra++) {
      indices[indiceAmostra] = indiceAmostra;
    }
    // Variável para armazenar a perda (erro) geral computada
    float perdas = 0.0f;
    // Inicia o loop de treinamento iterando por todas as épocas definidas
    for (int epoca = 0; epoca < epochs; epoca++) {

      // Embaralha as amostras (Fisher-Yates)
      for (int indiceEmbaralhamento = numeroTotalAmostras - 1; indiceEmbaralhamento > 0; indiceEmbaralhamento--) {
        int indiceAleatorio = (int)(esp_random() % (uint32_t)(indiceEmbaralhamento + 1));
        int indiceTemporario = indices[indiceEmbaralhamento];
        indices[indiceEmbaralhamento] = indices[indiceAleatorio];
        indices[indiceAleatorio] = indiceTemporario;
      }
      // Inicializa a variável para acumular a perda total ocorrida na época atual
      float soma_perda_epoca = 0.0f;

      // Percorre as amostras em lotes
      for (int inicioLote = 0; inicioLote < numeroTotalAmostras; inicioLote += tamanho_lote) {
        int fimLote = min(inicioLote + tamanho_lote, numeroTotalAmostras);
        int numeroAmostrasLote = fimLote - inicioLote;
        // Inicializa o acumulador de erro quadrático e o array de gradientes para o lote atual
        float soma_erro2 = 0.0f;
        float gradientes[3] = {0.0f, 0.0f, 0.0f};

        // Acumula erro e gradientes do lote chamando a função do Calculos.h
        for (int indiceLote = inicioLote; indiceLote < fimLote; indiceLote++) {
          int indiceAmostra = indices[indiceLote];
          processarAmostraLote(
            medicoes_carga[indiceAmostra],
            medicoes_distancia[indiceAmostra],
            gasto_viagens[indiceAmostra],
            pesos, vies, gradientes, soma_erro2
          );
        }

        // Perda MSE do lote e acúmulo da perda da época
        float perda_lote = calcularMSE(soma_erro2, numeroAmostrasLote);
        soma_perda_epoca += perda_lote * numeroAmostrasLote;

        // Atualização dos parâmetros pelo Adam via Calculos.h
        otimizadorAdam.atualizarParametros(pesos, vies, gradientes, numeroAmostrasLote);
      }

      // Perda total da época
      perdas = calcularMSE(soma_perda_epoca, numeroTotalAmostras);
    }

    // ==================================================================================================== //

    // Etapa 6 & 7: Previsão e Salvamento no CSV

    // Calcula a previsão do gasto com base na carga e distância usando os parâmetros treinados
    float gasto_previsto = calcularPrevisao(carga, distancia, pesos, vies);
    // Atribui a previsão a uma variável secundária para fins de validação
    float gasto_calculado = gasto_previsto;
    // Define as restrições físicas de porcentagem do gasto da bateria (0% a 100%)
    const float gasto_min = 0.0, gasto_max = 100.0;
    // Valida se o gasto calculado está dentro do limite real viável antes de salvar no registro
    if (!(gasto_min <= gasto_calculado && gasto_calculado <= gasto_max)) {
      Serial.printf("Alerta: A previsão de gasto (%.2f%%) é fisicamente impossível. Possível ponto fora da curva. Dado descartado.\n", gasto_calculado);
    } else {
      // Salva os dados válidos (carga, distância e o gasto calculado) no arquivo CSV
      gravarRegistro(caminho, carga, distancia, gasto_calculado);
    }
    // Exibe os resultados finais com os hiperparâmetros ajustados, perda final e previsão de gasto
    mostrarResultados(carga, distancia, perdas, pesos, vies, gasto_previsto);
  }
}