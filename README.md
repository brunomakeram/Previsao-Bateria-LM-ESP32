# Previsão de Bateria com Machine Learning Embarcado (ESP32 e C/C++)

## 📌 O que o projeto faz

Este projeto embarca um modelo de aprendizado de máquina (previamente treinado) em um microcontrolador **ESP32**. O sistema utiliza os parâmetros matemáticos aprendidos na nuvem (pesos e vieses) para prever o consumo percentual de uma bateria diretamente no hardware (Edge Computing), relacionando os dados de peso/carga e distância.

Para fins de organização técnica, eficiência de memória e clareza de código, o desenvolvimento em C++ foi estruturado e modularizado didaticamente em arquivos separados, dividindo as responsabilidades lógicas (desde a captura dos dados até a inferência preditiva).

---

## 💻 Ambiente de Desenvolvimento

- **Fase de Treinamento:** Algoritmo originalmente treinado em nuvem usando Python/TensorFlow para extração dos parâmetros.
- **Fase de Aplicação / Embarcados:** Desenvolvido e compilado localmente na **Arduino IDE** (ou VS Code com PlatformIO), focando em baixo nível (C/C++) e otimização de execução para o ESP32.

---

## 📂 Estrutura do Projeto

O código-fonte está dividido em 6 arquivos principais dentro do diretório do projeto:

- `Iniciar/Iniciar.ino`: Arquivo-fonte principal que orquestra o fluxo de inicialização (`setup`) e repetição (`loop`) do microcontrolador.
- `Iniciar/Calculos.h`: Responsável pelo processamento matemático dos dados, garantindo as validações físicas.
- `Iniciar/Dataset.h`: Armazena as variáveis de escopo, limites de segurança e constantes (como limites de distância e carga permitidas).
- `Iniciar/Leitura.h`: Gerencia a captação das entradas e comunicação com o usuário/sensores.
- `Iniciar/Modelo.h`: Contém o coração da inteligência artificial: a arquitetura do modelo convertido contendo os pesos e viés para a inferência matemática.
- `Iniciar/Prints.h`: Módulo focado exclusivamente na formatação e exibição visual dos resultados via Monitor Serial.

---

## 🚀 Como Usar

1. Clone o repositório em sua máquina.

2. Abra o arquivo principal `Iniciar.ino` utilizando a Arduino IDE. (Isso abrirá automaticamente todos os arquivos `.h` nas abas superiores).

3. Certifique-se de ter a placa da família **ESP32** instalada e selecionada no seu Gerenciador de Placas.

4. Conecte o ESP32 ao computador via USB, selecione a porta `COM` correta e clique em **Carregar (Upload)**.

5. Abra o **Monitor Serial** (configure a velocidade de baud rate compatível, ex: `115200`) para interagir com o sistema e visualizar as previsões de consumo de bateria em tempo real.

---

## 💻 Código-Fonte

Abaixo está um resumo da estrutura do `Iniciar.ino`, demonstrando o fluxo limpo de como as etapas práticas interagem usando a arquitetura modular:

```cpp

// // Importa a biblioteca
#include <Arduino.h>

// Inclui os arquivos do projeto: dados, modelo, leitura e calculos
#include "Dataset.h"
#include "Modelo.h"
#include "Leitura.h"
#include "Calculos.h"

// Roda uma vez ao ligar a placa ESP32
void setup() {
  // Inicia a comunicação serial a 115200 baud
  Serial.begin(115200);
  // Espera 2 segundos pro monitor serial abrir
  delay(2000);
  // Monta o armazenamento e garante que o arquivo CSV exista
  montarArmazenamento(caminho);
}

// Executado continuamente, permitindo novas consultas
void loop() {

  // Roda o modelo a cada ciclo do loop
  executarModelo();
  // Avisa que tá prono pra nova consulta
  Serial.println("\n================================");
  Serial.println("Pronto para uma nova consulta!");
  Serial.println("================================\n");
  // Espera meio segundo antes do próximo ciclo
  delay(500);
}

```

---

## 🖼️ Imagens do Projeto

Resultados visuais da execução da inferência rodando nativamente no ESP32:

### Saída do resultado no Monitor Serial

![Saída do resultado do Treino](imagens/resultado.jpg)

---

## 🧠 O que aprendi durante o desenvolvimento do projeto

1. **TinyML e Edge Computing (IA no Hardware):**
Entendimento profundo de como modelos treinados em linguagens de alto nível (Python/TensorFlow) operam matematicamente por baixo dos panos. Aprendi que uma rede neural com uma camada densa simples pode ser transcrita para uma equação linear direta ($y = a*x_1 + b*x_2 + c$) no C++. Isso permite inferências imediatas no hardware, dispensando conexões com a internet ou bibliotecas pesadas de Deep Learning no microcontrolador.

2. **Modularização em C/C++:**
Diferente da execução linear contínua do Google Colab, desenvolver em C++ exigiu fragmentar as regras do negócio. O uso inteligente de *Header Files* (`.h`) permitiu manter o escopo das variáveis seguro e o arquivo principal (`.ino`) extremamente limpo e voltado apenas para o fluxo de execução (Setup e Loop). Isso facilitou muito a manutenção do código.

3. **Otimização de Memória e Validação de Dados:**
A transição para hardware limitado (ESP32) me forçou a ser rigoroso com a validação das entradas e cálculos percentuais, prevenindo estouro de variáveis e assegurando que os limites físicos (como carga entre 1.0kg e 100.0kg) jamais quebrassem a estabilidade do sistema em tempo de execução.

---

## 🛠️ Stack Tecnológica e Referências

Para manter o código limpo, as bibliotecas, diretivas e tipos utilizados foram documentados abaixo:

### 1. Estrutura e Controle (`C++ e Arduino Framework`)
- **`#include "Arquivo.h"`**: Diretiva de pré-processamento usada para ligar (linkar) os arquivos de cabeçalho modulares, dividindo a arquitetura do firmware logicamente.
- **Tipagem Forte (`float`, `int`)**: Diferente do Python, a gestão correta da precisão dos números com pontos flutuantes (`float`) no C++ foi fundamental para que as equações mantivessem as frações numéricas geradas pelos pesos do Keras sem truncamento precoce.

### 2. Comunicação e Interface (`Serial`)
- **`Serial.begin(115200)`**: Inicia a comunicação UART para depuração de código e visualização da interface pelo Monitor Serial, usando uma taxa de transferência rápida de bits.
- **`Serial.println()` / `Serial.print()`**: Funções responsáveis pela saída dos relatórios formatados em `Prints.h`.

### 3. 📖 Para ver cada termo do código com explicação e link, consulte o: [Glossário](GLOSSARIO.md).

---

🚀 Próximos projetos  

Outros projetos serão postados em breve, com arquiteturas diferentes e níveis de complexidade maiores unindo Nuvem e Sistemas Embarcados.

👤 Autor: Bruno Zucker
