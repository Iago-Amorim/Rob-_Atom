#include <WiFi.h>
#include <WebServer.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Configurações de Rede Wi-Fi
const char* WIFI_SSID = "Robô_Atom";
const char* WIFI_PASSWORD = "GalinhaDeOuro";

// Configuração dos motores
const int MOTOR_PINS[4][2] = {
    {16, 17},   // Motor 1
    {18, 19},   // Motor 2
    {21, 22},   // Motor 3
    {23, 25}    // Motor 4
};

int Motor_Status[4] = {0, 0, 0, 0};

WebServer server(80);

// Página HTML para controle dos motores
String generateHtmlPage() {
    String html ="<!DOCTYPE html> <html lang=\"pt-BR\"> <head> <meta charset=\"UTF-8\" /> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" /> <title>Projeto Robô Atom</title> <style> :root { --bg-color: #e3f2fd; --text-color: #1e293b; --header-footer-color: #90caf9; --header-footer-text-color: #0d47a1; --button-color: #2c89b7; --button-hover-color: #0b4b6b; --button-ativado-color: #2cb731; --button-ativado-hover-color: #106b0b; --card-bg-color: #f3f4f6; --card-border-color: #546e7a; --modal-bg-color: #e3f2fd; --modal-border-color: #546e7a; --close-btn-color: #b71c1c; --close-btn-hover-color: #d32f2f; } * { margin: 0; padding: 0; box-sizing: border-box; } body { background-color: var(--bg-color); color: var(--text-color); font-family: Arial, sans-serif; width: 100vw; height: 100vh; display: flex; flex-direction: column; } header { background-color: var(--header-footer-color); color: var(--header-footer-text-color); width: 100%; padding: 1em; display: flex; align-items: center; justify-content: space-between; } header h1 { text-align: center; flex: 1; font-size: 1.5em; } header button { background-color: var(--button-color); color: #f1f5f9; padding: 0.8em 1.5em; border: 0; cursor: pointer; transition: background-color 0.3s ease; font-size: 1em; border-radius: 0.5em; } header button:hover { background-color: var(--button-hover-color); } main { flex: 1; padding: 2em 3em; display: flex; flex-wrap: wrap; gap: 2em; justify-content: center; align-items: flex-start; } main div { background-color: var(--card-bg-color); width: calc(20% - 1em); padding: 1.5em 1em 1em 1em; border-radius: 0.5em; display: flex; flex-direction: column; align-items: center; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); border: 1px solid var(--card-border-color); } main div div { padding: none; box-shadow: none; border: none; } main h2 { font-size: 1.2em; text-align: center; border-bottom: 2px solid var(--card-border-color); width: 100%; padding-bottom: 0.5em; color: var(--text-color); } .btn_aumentar, .btn_diminuir { font-size: 2em; width: 4em; height: 4em; margin: 0.5em; cursor: pointer; transition: background-color 0.3s ease; background-color: var(--button-color); color: #f1f5f9; border: none; border-radius: 5%; } .btn_aumentar:hover, .btn_diminuir:hover { background-color: var(--button-hover-color); } .btn_aumentar { rotate: -90deg; } .btn_diminuir { rotate: 90deg; } .btn_ativado:hover, .btn_ativado:active { background-color: var(--button-ativado-hover-color) !important; } footer { padding: 1em; background-color: var(--header-footer-color); text-align: center; color: var(--header-footer-text-color); } .modal { position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%); background-color: var(--modal-bg-color); padding: 2em; border-radius: 0.75em; box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3); width: 90%; max-width: 500px; z-index: 1000; display: none; } .modal.active { display: block; } .modal-header { display: flex; justify-content: space-between; align-items: center; border-bottom: 2px solid var(--modal-border-color); padding-bottom: 1em; margin-bottom: 1.5em; } .modal-header h2 { font-size: 1.5em; color: var(--text-color); } .modal-header .close-btn { background-color: var(--close-btn-color); color: #fff; border: none; font-size: 1.2em; cursor: pointer; border-radius: 5%; width: 2em; height: 2em; padding: 0.1em 0.5em; display: flex; justify-content: center; align-items: center; transition: background-color 0.3s ease; } .modal-header .close-btn:hover { background-color: var(--close-btn-hover-color); } .modal-section { padding-bottom: 0.5em; } .modal-section h3 { padding-bottom: 0.2em; } .modal-section p { padding-left: 1em; } ";
    html +="@media (max-width: 1000px) { main { gap: 1em; } main div { width: calc(24% - 0.5em); padding: 1em 1em 0.5em 1em; } main div div { padding: none; box-shadow: none; border: none; } } ";
    html +="@media (max-width: 700px) { main div { width: 100%; } .modal { width: 95%; } .modal-header h2 { font-size: 1.2em; } } </style> </head> <body> <header> <h1>Projeto Robô Atom</h1> <button aria-label=\"Sobre\">Sobre</button> </header> <main> <div> <h2>Articulação 1</h2> <div id=\"articulacao_1\"> <button class=\"btn_aumentar\" data-route=\"/motor1_fwd\" aria-label=\"Aumentar\" ></button> <button class=\"btn_diminuir\" data-route=\"/motor1_rev\" aria-label=\"Diminuir\" ></button> </div> </div> <div> <h2>Articulação 2</h2> <div id=\"articulacao_2\"> <button class=\"btn_aumentar\" data-route=\"/motor2_fwd\" aria-label=\"Aumentar\" ></button> <button class=\"btn_diminuir\" data-route=\"/motor2_rev\" aria-label=\"Diminuir\" ></button> </div> </div> <div> <h2>Articulação 3</h2> <div id=\"articulacao_3\"> <button class=\"btn_aumentar\" data-route=\"/motor3_fwd\" aria-label=\"Aumentar\" ></button> <button class=\"btn_diminuir\" data-route=\"/motor3_rev\" aria-label=\"Diminuir\" ></button> </div> </div> <div> <h2>Garra</h2> <div id=\"garra\"> <button class=\"btn_aumentar\" data-route=\"/motor4_fwd\" aria-label=\"Aumentar\" ></button> <button class=\"btn_diminuir\" data-route=\"/motor4_rev\" aria-label=\"Diminuir\" ></button> </div> </div> </main> <div class=\"modal\" id=\"infoModal\"> <div class=\"modal-header\"> <h2>Projeto A3 de Eletrônica Industrial da Universidade Salvador – UNIFACS</h2> <button class=\"close-btn\" aria-label=\"Fechar modal\">X</button> </div> <div class=\"modal-content\"> <div class=\"modal-section\"> <h3>Professores:</h3> <p>Euclério Barbosa Ornellas Filho</p> <p>Paulo Cesar da Silva Emanuel</p> </div> <div class=\"modal-section\"> <h3>Alunos:</h3> <p>Marcos Caique Campelo de Miranda</p> <p>Maria Eduarda Santos da Silva</p> <p>Júlia Moreira Silva dos Santos</p> <p>Gabriel Sapucaia Carvalho</p> <p>Iago Assunção Amorim</p> </div> </div> </div> <footer> <p>&copy; 2024 Projeto Atom.</p> </footer> <script> let currentInterval = null; async function controlMotor(route) { try { const response = await fetch(route); if (!response.ok) { throw new Error(`Erro na requisição para ${route}: ${response.status}`); } return await response.text(); } catch (error) { console.error(\"Erro ao controlar motor:\", error); } } function startRequests(button) { stopCurrentRequests(); const route = button.dataset.route; controlMotor(route); currentInterval = setInterval(() => controlMotor(route), 100); } function stopCurrentRequests() { if (currentInterval) { clearInterval(currentInterval); currentInterval = null; } } function setupMotorButtons() { const buttons = document.querySelectorAll(\".btn_aumentar, .btn_diminuir\"); buttons.forEach((button) => { button.addEventListener(\"mousedown\", () => startRequests(button)); button.addEventListener(\"mouseup\", stopCurrentRequests); button.addEventListener(\"mouseleave\", stopCurrentRequests); button.addEventListener(\"touchstart\", (event) => { event.preventDefault(); startRequests(button); }); }); document.addEventListener(\"touchend\", stopCurrentRequests); document.addEventListener(\"touchcancel\", stopCurrentRequests); window.addEventListener(\"mouseup\", stopCurrentRequests); } async function atualizarBotoes() { try { const response = await fetch(\"/informacao\"); if (!response.ok) { throw new Error(`Erro na requisição de atualização: ${response.status}`); } const data = await response.json(); const botoes = [ document.querySelectorAll(\"#articulacao_1 button\"), document.querySelectorAll(\"#articulacao_2 button\"), document.querySelectorAll(\"#articulacao_3 button\"), document.querySelectorAll(\"#garra button\"), ]; Object.keys(data).forEach((key, index) => { const valor = data[key]; if (botoes[index]) { botoes[index].forEach((button) => { button.classList.toggle(\"btn_ativado\", valor === 1); }); } }); } catch (error) { console.error(\"Erro ao buscar dados:\", error); } } function addSvgToButtons() { const buttons = document.querySelectorAll(\".btn_aumentar, .btn_diminuir\"); const svgIcon = ` <svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 100 100\"> <g> <path d=\"M31.356,25.677l38.625,22.3c1.557,0.899,1.557,3.147,0,4.046l-38.625,22.3c-1.557,0.899-3.504-0.225-3.504-2.023V27.7C27.852,25.902,29.798,24.778,31.356,25.677z\"/> </g> </svg>`; buttons.forEach((button) => { button.innerHTML = svgIcon + button.innerHTML; }); } function setupAutoUpdate(interval = 500) { atualizarBotoes(); setInterval(atualizarBotoes, interval); } function setupModalButtons() { document.querySelector(\"header button\").addEventListener(\"click\", () => { document.getElementById(\"infoModal\").classList.toggle(\"active\"); }); document.querySelector(\".modal button\").addEventListener(\"click\", () => { document.getElementById(\"infoModal\").classList.remove(\"active\"); }); } setupModalButtons(); addSvgToButtons(); setupAutoUpdate(); document.addEventListener(\"DOMContentLoaded\", setupMotorButtons); </script> </body> </html>";
    return html;
}

// Variáveis Globais
TaskHandle_t desativaBotaoTaskHandle = NULL; // Handle para a tarefa
SemaphoreHandle_t mutex;                    // Mutex para proteger variáveis compartilhadas

// Função que será executada na tarefa para desativar o botão após 500ms
void desativaBotaoTask(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(500)); // Aguarda 500 ms

    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        for (int motorIndex = 0; motorIndex < 4; motorIndex++) {
            if (Motor_Status[motorIndex]) {
                //Serial.print("Motor ");
                //Serial.print(motorIndex + 1);
                //Serial.println(" Desligado!");

                digitalWrite(MOTOR_PINS[motorIndex][0], LOW);
                digitalWrite(MOTOR_PINS[motorIndex][1], LOW);

                Motor_Status[motorIndex] = 0;
                //logMotorStatus();
            }
        }
        xSemaphoreGive(mutex); // Libera o mutex após a execução
    }

    desativaBotaoTaskHandle = NULL; // Redefine o handle da tarefa
    //Serial.println("Tarefa de desativação concluída!");
    vTaskDelete(NULL); // Deleta a tarefa atual
}

// Função para retornar o status dos motores em JSON
void handleMotorStatus() {
  String jsonResponse = "{";
  for (int i = 0; i < 4; i++) {
    jsonResponse += "\"" + String(i + 1) + "\":" + String(Motor_Status[i]);
    if (i < 3) jsonResponse += ",";  // Adiciona vírgula entre os valores
  }
  jsonResponse += "}";

  server.send(200, "application/json", jsonResponse);  // Envia a resposta JSON
}

// Exibe o status de cada pino dos motores no console
void logMotorStatus() {
    Serial.println("");
    for (int i = 0; i < 4; i++) {
        Serial.print("Motor ");
        Serial.print(i + 1);
        Serial.println(Motor_Status[i] ? ": Ligado" : ": Desligado");
        for (int j = 0; j < 2; j++) {
            int pinState = digitalRead(MOTOR_PINS[i][j]);
            Serial.print("Motor ");
            Serial.print(i + 1);
            Serial.print(" Pino ");
            Serial.print(MOTOR_PINS[i][j]);
            Serial.print(": ");
            Serial.println(pinState == HIGH ? "HIGH" : "LOW");
        }
    }
}

// Configura o modo de cada pino para os motores
void configureMotorPins() {
    for (int i = 0; i < 4; i++) {
        pinMode(MOTOR_PINS[i][0], OUTPUT);
        pinMode(MOTOR_PINS[i][1], OUTPUT);
        digitalWrite(MOTOR_PINS[i][0], LOW);
        digitalWrite(MOTOR_PINS[i][1], LOW);
    }
}

// Configura as rotas do servidor para controle dos motores
void setupServerRoutes() {
    server.on("/", []() {
        server.send(200, "text/html", generateHtmlPage());
    });

    // Rota para mandar informação dos motores
    server.on("/informacao", HTTP_GET, handleMotorStatus);

    // Rotas para controlar cada motor
    server.on("/motor1_fwd", []() { controlMotor(0, HIGH, LOW); server.send(200); });
    server.on("/motor1_rev", []() { controlMotor(0, LOW, HIGH); server.send(200); });
    server.on("/motor2_fwd", []() { controlMotor(1, HIGH, LOW); server.send(200); });
    server.on("/motor2_rev", []() { controlMotor(1, LOW, HIGH); server.send(200); });
    server.on("/motor3_fwd", []() { controlMotor(2, HIGH, LOW); server.send(200); });
    server.on("/motor3_rev", []() { controlMotor(2, LOW, HIGH); server.send(200); });
    server.on("/motor4_fwd", []() { controlMotor(3, HIGH, LOW); server.send(200); });
    server.on("/motor4_rev", []() { controlMotor(3, LOW, HIGH); server.send(200); });
}

// Controla o estado de um motor específico
void controlMotor(int motorIndex, int state1, int state2) {
    //Serial.print("Motor ");
    //Serial.print(motorIndex + 1);
    //Serial.println(state1 ? " Avanço!" : " Retorno!");

    digitalWrite(MOTOR_PINS[motorIndex][0], state1);
    digitalWrite(MOTOR_PINS[motorIndex][1], state2);

    for (int i = 0; i < 4; i++) {
        if ((Motor_Status[i]) && (i != motorIndex)) {
            //Serial.print("Motor ");
            //Serial.print(i + 1);
            //Serial.println(" Desligado!");

            digitalWrite(MOTOR_PINS[i][0], LOW);
            digitalWrite(MOTOR_PINS[i][1], LOW);

            Motor_Status[i] = 0;
            //logMotorStatus();
        }
    }

    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        Motor_Status[motorIndex] = 1;
        logMotorStatus();
        xSemaphoreGive(mutex); // Libera o mutex
    }

    // Se já existir uma tarefa para desativar o botão, excluí-la
    if (desativaBotaoTaskHandle != NULL) {
        vTaskDelete(desativaBotaoTaskHandle);
        desativaBotaoTaskHandle = NULL;
        //Serial.println("Tarefa anterior deletada!");
    }

    // Criar uma nova tarefa para desativar o botão após 500ms
    xTaskCreatePinnedToCore(
        desativaBotaoTask,          // Função da tarefa
        "DesativaBotaoTask",        // Nome da tarefa
        1024,                       // Tamanho da pilha (em bytes)
        NULL,                       // Parâmetro para passar à tarefa
        1,                          // Prioridade da tarefa
        &desativaBotaoTaskHandle,   // Handle da tarefa
        1                           // Núcleo onde a tarefa será executada (core 1)
    );
}

// Configuração inicial do ESP32
void setup() {
    Serial.begin(115200);

    // Inicializa Wi-Fi como ponto de acesso
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Ponto de acesso iniciado!");

    // Inicializa o mutex
    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        Serial.println("Erro ao criar o mutex!");
        while (true); // Trava o programa se o mutex falhar
    }

    configureMotorPins();
    setupServerRoutes();
    server.begin();
    Serial.println("Servidor iniciado!");
}

// Função principal de loop
void loop() {
    server.handleClient();
}
