// Garante que este arquivo seja incluído só uma vez por compilação
#pragma once
// Importa as bibliotecas
#include <Arduino.h>
#include <math.h>

// ==================================================================================================== //

// Previsão e métricas de erro (MSE)

// Calcula a previsão linear (y = a*carga + b*distancia + vies)
inline float calcularPrevisao(float carga, float distancia, const float* pesos, float vies) {
  return pesos[0] * carga + pesos[1] * distancia + vies;
}

// Calcula o Erro Quadrático Médio (MSE) dividindo a soma dos erros pelo total de amostras
inline float calcularMSE(float somaErrosQuadrados, int totalAmostras) {
  if (totalAmostras <= 0) return 0.0f;
  return somaErrosQuadrados / (float)totalAmostras;
}

// ==================================================================================================== //

// Cáculo de gradientes

// Processa uma única amostra: calcula a previsão, acumula o erro quadrático e os gradientes dos parâmetros
inline void processarAmostraLote(
  float carga, 
  float distancia, 
  float gastoReal, 
  const float* pesos, 
  float vies, 
  float* gradientes, 
  float& somaErroQuadrado
) {
  // Previsão do modelo para a amostra atual
  float gastoPrevisto = calcularPrevisao(carga, distancia, pesos, vies);

  // Erro entre a previsão e o valor real
  float erro = gastoPrevisto - gastoReal;

  // Acumula o erro ao quadrado para o cálculo da perda (MSE)
  somaErroQuadrado += erro * erro;

  // Acumula os gradientes para o peso da carga (0), peso da distância (1) e viés (2)
  gradientes[0] += 2.0f * erro * carga;
  gradientes[1] += 2.0f * erro * distancia;
  gradientes[2] += 2.0f * erro;
}

// ==================================================================================================== //

// Otimizador Adam

struct OtimizadorAdam {
  // Taxa de aprendizado e fatores de decaimento dos momentos
  float taxaAprendizado;
  float decaimentoPrimeiroMomento;
  float decaimentoSegundoMomento;
  float margemEstabilidadeNumerica;

  // Contador de atualizações realizadas
  long numeroAtualizacoes;

  // Médias móveis dos gradientes e dos gradientes ao quadrado para cada parâmetro
  float mediaMovelGradientes[3];
  float mediaMovelGradientesQuadrados[3];

  // Inicializa os parâmetros do otimizador Adam
  void inicializar(float lr = 0.05f, float beta1 = 0.9f, float beta2 = 0.999f, float eps = 1e-7f) {
    taxaAprendizado = lr;
    decaimentoPrimeiroMomento = beta1;
    decaimentoSegundoMomento = beta2;
    margemEstabilidadeNumerica = eps;
    numeroAtualizacoes = 0;

    for (int indiceParametro = 0; indiceParametro < 3; indiceParametro++) {
      mediaMovelGradientes[indiceParametro] = 0.0f;
      mediaMovelGradientesQuadrados[indiceParametro] = 0.0f;
    }
  }

  // Recebe os gradientes acumulados, calcula a média pelo tamanho do lote e atualiza os pesos e o viés
  void atualizarParametros(float* pesos, float& vies, float* gradientes, int numeroAmostrasLote) {
    if (numeroAmostrasLote <= 0) return;

    // Normaliza os gradientes dividindo pelo número de amostras do lote
    for (int indiceParametro = 0; indiceParametro < 3; indiceParametro++) {
      gradientes[indiceParametro] /= (float)numeroAmostrasLote;
    }

    // Incrementa o contador de atualizações do Adam
    numeroAtualizacoes++;

    // Calcula as correções de viés
    float correcaoSegundoMomento = sqrtf(1.0f - powf(decaimentoSegundoMomento, (float)numeroAtualizacoes));
    float correcaoPrimeiroMomento = 1.0f - powf(decaimentoPrimeiroMomento, (float)numeroAtualizacoes);
    float taxaAprendizadoCorrigida = taxaAprendizado * correcaoSegundoMomento / correcaoPrimeiroMomento;

    // Agrupa os ponteiros dos parâmetros para atualizar no laço
    float* parametros[3] = {&pesos[0], &pesos[1], &vies};

    for (int indiceParametro = 0; indiceParametro < 3; indiceParametro++) {
      // Atualiza a média móvel dos gradientes (1º momento)
      mediaMovelGradientes[indiceParametro] =
        decaimentoPrimeiroMomento * mediaMovelGradientes[indiceParametro] +
        (1.0f - decaimentoPrimeiroMomento) * gradientes[indiceParametro];

      // Atualiza a média móvel dos gradientes ao quadrado (2º momento)
      mediaMovelGradientesQuadrados[indiceParametro] =
        decaimentoSegundoMomento * mediaMovelGradientesQuadrados[indiceParametro] +
        (1.0f - decaimentoSegundoMomento) * gradientes[indiceParametro] * gradientes[indiceParametro];

      // Calcula o valor do ajuste
      float ajusteParametro =
        taxaAprendizadoCorrigida * mediaMovelGradientes[indiceParametro] /
        (sqrtf(mediaMovelGradientesQuadrados[indiceParametro]) + margemEstabilidadeNumerica);

      // Aplica o ajuste no parâmetro correspondente
      *(parametros[indiceParametro]) -= ajusteParametro;
    }
  }
};