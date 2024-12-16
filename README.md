# Projeto Robô Atom

## Visão Geral
Este é o código para o **Projeto Robô Atom**, desenvolvido como parte de um projeto acadêmico na Universidade Salvador (UNIFACS). Ele utiliza uma placa ESP32 para controlar um sistema de motores através de uma interface web, permitindo manipular articulações e uma garra em um braço robótico. A interface foi projetada para ser responsiva e acessível em dispositivos móveis e desktops.

## Imagem
![Projeto_Atom](https://github.com/user-attachments/assets/a65db566-fc82-4508-947e-e1d5dc7038c9)


## Funcionalidades
- Controle remoto de quatro motores (três articulações e uma garra).
- Interface web responsiva com botões para aumentar e diminuir os movimentos.
- Atualização automática do status dos motores.
- Controle seguro dos motores com bloqueio automático após 500 ms de inatividade.
- API para monitoramento do status dos motores.

## Tecnologias Utilizadas
- **Plataforma:** ESP32
- **Rede:** Wi-Fi
- **Linguagem:** C++ (Arduino)
- **Frameworks:**
  - FreeRTOS para tarefas concorrentes.
  - WebServer para a criação do servidor HTTP.
- **Interface Web:** HTML, CSS e JavaScript.

## Configuração do Projeto
### Requisitos
- Placa ESP32.
- IDE Arduino configurada para ESP32.
- Motores conectados conforme a tabela abaixo:

| Motor      | Pinos ESP32 |
|------------|-------------|
| Motor 1    | 16, 17      |
| Motor 2    | 18, 19      |
| Motor 3    | 21, 22      |
| Motor 4    | 23, 25      |

### Como Configurar
1. **Configurar o ambiente de desenvolvimento:**
   - Baixe e instale a [IDE Arduino](https://www.arduino.cc/en/software).
   - Instale o pacote de suporte para ESP32 na IDE Arduino seguindo [este guia](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

2. **Clonar o repositório:**
   ```bash
   git clone https://github.com/Iago-Amorim/Robo_Atom
   ```

3. **Abrir o código na IDE Arduino:**
   - Abra o arquivo principal do projeto (`EletronicaIndustrial_BracoRobotico_A3.ino`).

4. **Configurar as credenciais Wi-Fi:**
   - Edite as variáveis `WIFI_SSID` e `WIFI_PASSWORD` no código com o nome e a senha da sua rede Wi-Fi.

5. **Upload para o ESP32:**
   - Conecte o ESP32 ao computador e envie o código para a placa.

6. **Conectar-se à rede Wi-Fi criada pelo ESP32:**
   - Nome da rede: `Robô_Atom`
   - Senha: `GalinhaDeOuro`

7. **Acessar a interface web:**
   - Abra um navegador e acesse o endereço IP do ESP32 (normalmente `192.168.4.1`).

## Estrutura do Código
### Principais Componentes
- **`generateHtmlPage`:** Função que gera a interface web.
- **`configureMotorPins`:** Configura os pinos do ESP32 para controle dos motores.
- **`handleMotorStatus`:** Retorna o status dos motores no formato JSON.
- **`desativaBotaoTask`:** Tarefa do FreeRTOS que desliga os motores após 500 ms de inatividade.
- **Rotas HTTP:**
  - `/`: Interface web.
  - `/informacao`: Status dos motores (JSON).
  - `/motor[N]_fwd` e `/motor[N]_rev`: Controle individual dos motores.

## Contribuições
Contribuições são bem-vindas! Para contribuir:
1. Crie um fork do repositório.
2. Crie um branch para suas modificações:
   ```bash
   git checkout -b minha-modificacao
   ```
3. Envie suas alterações:
   ```bash
   git commit -m "Minha modificação"
   git push origin minha-modificacao
   ```
4. Abra um pull request no GitHub.

## Licença
Este projeto está licenciado sob a licença MIT. Consulte o arquivo `LICENSE` para mais detalhes.

## Créditos
Projeto realizado por:
- Marcos Caique Campelo de Miranda
- Maria Eduarda Santos da Silva
- Júlia Moreira Silva dos Santos
- Gabriel Sapucaia Carvalho
- Joao Henrique Araujo Carneiro da Cunha
- Iago Assunção Amorim

