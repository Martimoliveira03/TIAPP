# WAT'R – Sistema Sustentável de Monitoramento de Água

Este repositório contém os arquivos-fonte, diagramas e documentação relacionados ao projeto **WAT'R**, desenvolvido no âmbito da disciplina de Tecnologias de Interface. O objetivo do projeto é incentivar hábitos mais conscientes de uso da água, oferecendo feedback em tempo real e insights sobre o consumo a longo prazo, por meio de um sistema de monitoramento inteligente e de baixo custo.

---

## 📌 Descrição do Projeto

O WAT'R combina componentes de hardware e software para monitorar o consumo de água durante o banho, oferecendo ao usuário um retorno imediato (via som, luz e display) e também um histórico visual de uso. O sistema utiliza um **microcontrolador ESP32** para captar os dados de vazão em tempo real e os envia para um **servidor Node.js**, onde são armazenados em formato JSON e posteriormente exibidos em uma interface simples.

O sistema oferece:
- Feedback em tempo real (LCD, LED e buzzer)
- Modo de contagem dupla (progressiva ou regressiva)
- Transmissão de dados via Wi-Fi
- Armazenamento em servidor utilizando arquivos JSON
- Visualização do histórico de consumo em **gráficos de linha**

O objetivo é **promover consciência** e apoiar a **mudança de comportamento** por meio de design de interação, feedback embarcado e visualização de dados.

---

## 📁 Estrutura do Repositório

```bash
WATR/
├── docs/                 # Relatório final e versões intermediárias
│
├── res/                  # Recursos visuais utilizados no relatório
│
├── src/                  # Código-fonte (firmware ESP32 e frontend)
│   ├── esp32/            # Código Arduino adaptado para ESP32
│   └── assets/           # Ícones e imagens da aplicação
│
├── watr-server/          # Código do servidor Node.js e base de dados JSON
├── .gitignore
├── LICENSE
├── README.md
└── CODE_OF_CONDUCT.md
