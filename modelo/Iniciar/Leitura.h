// Garante que este arquivo seja incluído só uma vez por compilação
#pragma once
// Importa as bibliotecas
#include <Arduino.h>
#include <LittleFS.h>
#include <vector>

// Inclui os arquivos do projeto: dataset e modelo
#include "Dataset.h"
#include "Modelo.h"

// Estrutura que substitui o DataFrame do pandas, com as colunas "Carga", "Distância" e "Gasto"
struct Dataset {
  std::vector<float> carga;
  std::vector<float> distancia;
  std::vector<float> gasto;
};

// Monta o armazenamento (substitui drive.mount) e cria o arquivo CSV na primeira execução

// Os dados ficam na flash e continuam salvos depois de reiniciar a placa
inline bool montarArmazenamento(const char* caminho) {
  // true = formata a partição se ainda não estiver formatada
  if (!LittleFS.begin(true)) {
    Serial.println("Erro: falha ao montar o LittleFS.");
    return false;
  }
  // Verifica se o arquivo CSV já existe no armazenamento interno do ESP32
  if (!LittleFS.exists(caminho)) {
    // Abre o arquivo no modo escrita, criando um novo se ele não existir
    File arquivo = LittleFS.open(caminho, FILE_WRITE);
    if (!arquivo) {
      Serial.println("Erro: falha ao criar o arquivo do dataset.");
      // Retorna false, indicando que a operação falhou
      return false;
    }
    // Grava no arquivo o conteúdo inicial padrão do dataset, com os cabeçalhos e os dados de exemplo
    arquivo.print(DATASET_PADRAO);
    // Fecha o arquivo para garantir que os dados sejam gravados
    arquivo.close();
  }
  // Retorna verdadeiro, indicando que a operação deu certo
  return true;
}

// Lê o arquivo CSV (substitui pd.read_csv) e preenche as colunas "Carga", "Distância" e "Gasto"
inline bool lerDataset(const char* caminho, Dataset& dados) {
  File arquivo = LittleFS.open(caminho, FILE_READ);
  if (!arquivo) return false;

  // Guarda a posição de cada coluna no CSV, começando em -1 para indicar que ainda não foi encontrada
  int colunaCarga = -1, colunaDistancia = -1, colunaGasto = -1;
  bool cabecalho = true;

  // Repete enquanto ainda houver dados para ler no arquivo, uma linha por vez
  while (arquivo.available()) {
    // Lê uma linhaTabela do arquivo, até a quebra de linha
    String linhaTabela = arquivo.readStringUntil('\n');
    // Remove espaços e quebra de linha das pontas do texto
    linhaTabela.trim();
    // Se a linha estiver vazia, pula para a próxima
    if (linhaTabela.length() == 0) continue;

    // Divide a linhaTabela em campos separados por vírgula
    String camposLinha[8];
    // Variáveis para controlar a quantidade de campos encontrados e a posição inicial da busca na linha
    int totalCampos = 0, inicio = 0;
    
    // Repete a extração de campos até atingir o limite do array (8 campos)
    while (totalCampos < 8) {
      // Procura a posição da próxima vírgula na linha, a partir da posição inicial
      int virgula = linhaTabela.indexOf(',', inicio);
      
      // Se não encontrar mais nenhuma vírgula, extrai o último trecho da linha até o final e sai da repetição
      if (virgula < 0) { 
        camposLinha[totalCampos++] = linhaTabela.substring(inicio); 
        break; 
      }
      
      // Extrai o texto que está entre a posição inicial e a vírgula encontrada, e guarda no array de campos
      camposLinha[totalCampos++] = linhaTabela.substring(inicio, virgula);
      // Atualiza a posição inicial para logo depois da vírgula, para buscar o próximo campo
      inicio = virgula + 1;
    }

    // Se estiver lendo a primeira linha (cabeçalho), identifica onde estão as colunas que importam
    if (cabecalho) {
      // Localiza as colunas pelo nome, percorrendo todos os campos lidos
      for (int indiceCampo = 0; indiceCampo < totalCampos; indiceCampo++) {
        camposLinha[indiceCampo].trim();
        // Marca a posição da coluna se o título corresponder ao nome esperado
        if (camposLinha[indiceCampo] == "Carga") colunaCarga = indiceCampo;
        else if (camposLinha[indiceCampo] == "Distância") colunaDistancia = indiceCampo;
        else if (camposLinha[indiceCampo] == "Gasto") colunaGasto = indiceCampo;
      }
      
      // Se alguma das três colunas essenciais não for encontrada, fecha o arquivo e retorna falso
      if (colunaCarga < 0 || colunaDistancia < 0 || colunaGasto < 0) {
        arquivo.close();
        return false;
      }
      
      // Marca que o cabeçalho já foi lido e pula para a leitura da próxima linha (os dados em si)
      cabecalho = false;
      continue;
    }

    // Extrai o valor do texto da posição correta da coluna, converte para número e guarda no final do vetor correspondente na estrutura de dados
    dados.carga.push_back(camposLinha[colunaCarga].toFloat());
    dados.distancia.push_back(camposLinha[colunaDistancia].toFloat());
    dados.gasto.push_back(camposLinha[colunaGasto].toFloat());
  }

  // Fecha o arquivo após ler todas as linhas e terminar o processamento
  arquivo.close();
  
  // Retorna verdadeiro se pelo menos um registro válido de gasto foi lido e guardado (o vetor não está vazio)
  return !dados.gasto.empty();
}

// Converte um número para texto no formato do CSV (ex.: 15 vira "15.0")
inline String formatarNumero(float valor) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%.9g", (double)valor);
  String texto = String(buffer);
  if (texto.indexOf('.') < 0 && texto.indexOf('e') < 0) texto += ".0";
  return texto;
}

// Grava um novo registro no fim do CSV (substitui to_csv com mode='a', header=False, index=False)
inline bool gravarRegistro(const char* caminho, float carga, float distancia, float gasto) {
  // Abre o arquivo no modo de adição (append), que grava os novos dados no final sem apagar o que já existe
  File arquivo = LittleFS.open(caminho, FILE_APPEND);
  if (!arquivo) return false;
  
  // Escreve os valores formatados separados por vírgula e termina com uma quebra de linha
  arquivo.print(formatarNumero(carga));
  arquivo.print(",");
  arquivo.print(formatarNumero(distancia));
  arquivo.print(",");
  arquivo.print(formatarNumero(gasto));
  arquivo.print("\n");
  
  // Fecha o arquivo para garantir que a gravação seja salva na memória flash
  arquivo.close();
  
  // Retorna verdadeiro, indicando que o registro foi gravado com sucesso
  return true;
}