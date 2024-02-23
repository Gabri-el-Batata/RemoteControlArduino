// C++ code
#include <IRremote.h>

// Constants
const int RECV_PIN = 10;
const float INITIAL_KP = 0;
const float INITIAL_KD = 0;

// Estados da corrida
enum RaceStates {
  WAITING_FOR_CATEGORY,
  WAITING_FOR_CALIBRATION,
  CALIBRATION,
  WAITING_FOR_RACE,
  RACE,
  COMPLETED
};

// Variables
RaceStates currentRaceState = WAITING_FOR_CATEGORY;
bool follower;
// Definition of PID parameters
float kP = INITIAL_KP;
float kD = INITIAL_KD;

const unsigned long THREE = 3977428736;
const unsigned long ONE = 4010852096;
const unsigned long TWO = 3994140416;
const unsigned long NINE = 3843735296;
const unsigned long ZERO = 4077698816;
const unsigned long LEFT_ARROW = 3977428736;
const unsigned long RIGHT_ARROW = 3977428736;


void handleRace(unsigned long code)
{
  if(code == 3977428736)
  {
  	estadoAtualCorr = CORRIDA;
  }
}

void handleCalibration(unsigned long code)
{
	if (code == 4010852096){
      		// Apertou 1 utiliza calibracao ja existente
      		Serial.println("Utilizando calibracao ja existente...");
      		estadoAtualCorr = AGUARDANDO_CORRIDA;
    	}else if (code == 3994140416)
    	{
      		// Apertou 2 comeca a calibrar
      		Serial.println("Calibrando...");
      		estadoAtualCorr = CALIBRACAO;
    	}
}

void handleCategory(unsigned long code)
{
	if (code == 3843735296)
    {
      // Se apertar 9 muda para o seguidor;
      Seguidor = true;
      estadoAtualCorr = AGUARDANDO_CALIBRACAO;
      Serial.println("Modo Seguidor.");
    } else if(code == 4077698816)
    {
      // Apertou 0 fica perseguidor
      Seguidor = false;
      estadoAtualCorr = CONCLUIDO;
      Serial.println("Modo Perseguidor.");
    }
}

void adjustPIDParameters(unsigned long code)
{
	 if(code == 4211392256)
     {
          	// Aumentando valor de kD
            kD -=0.05;
            Serial.print("Valor de kD: ");
            Serial.println(kD);
     }
     if(code == 4177968896)
     {
          	// Diminui valor de kD
            kD += 0.05;
            Serial.print("Valor de kD: ");
            Serial.println(kD);
     }
     if(code == 4111122176)
     {
          	// Aumenta valor de kP
            kP += 0.05;
            Serial.print("Valor de kP: ");
            Serial.println(kP);
     }
     if(code == 4144545536)
     {
          	// Diminui valor de kP
            kP -= 0.05;
            Serial.print("Valor de kP: ");
            Serial.println(kP);
     }
}

void readIr() {
  if (IrReceiver.decode()){
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    //Serial.println(code, DEC); // printar o codigo ir recebido
    IrReceiver.resume(); // receber proximo valor
    
    // Secao responsavel por escolher categoria
    if (estadoAtualCorr == AGUARDANDO_CATEGORIA)
    {
    	mudaCategoria(code);
    }
    
    // Secao para parte da corrida
    if (Seguidor)
    {
    	if (estadoAtualCorr == AGUARDANDO_CALIBRACAO){
      		carregarCriarCalib(code);
        }
    	if (estadoAtualCorr == AGUARDANDO_CORRIDA)
    	{
      		correr(code);
    	}
        
        // Mudando Parametros do PID
      	if (estadoAtualCorr == AGUARDANDO_CORRIDA || estadoAtualCorr == CORRIDA)
        {
          aumentaValoresPID(code);
    	}
     }
  }
}


void stateMachine() {
  switch(estadoAtualCorr){
    case (AGUARDANDO_CATEGORIA):
      digitalWrite(7, HIGH);
      break;
    case (AGUARDANDO_CALIBRACAO):
      break;
    case (CALIBRACAO):
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
      delay(2000);
      estadoAtualCorr = AGUARDANDO_CORRIDA;
      break; 
    case(AGUARDANDO_CORRIDA):
      break;
    case (CORRIDA):
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      Serial.println("Correndo...");
      delay(3000);
      estadoAtualCorr = CONCLUIDO;
      break;
    case (CONCLUIDO):
      digitalWrite(5, LOW);
      Serial.println("Concluido.");
      estadoAtualCorr = AGUARDANDO_CATEGORIA;
      break;
    default: Serial.println("Escolha uma categoria."); break;
  }
  
  switch(estadoAtualCorr){
  case (AGUARDANDO_CATEGORIA):
    Serial.println("AGUARDANDO_CATEGORIA");
    break;
  case AGUARDANDO_CALIBRACAO:
    Serial.println("AGUARDANDO_CALIBRACAO");
    break;
  case CALIBRACAO: 
    Serial.println("CALIBRACAO");
    break;
  case AGUARDANDO_CORRIDA: 
    Serial.println("AGUARDANDO_CORRIDA");
    break;
  case CORRIDA: 
    Serial.println("CORRIDA");
    break;
  case CONCLUIDO: 
    Serial.println("CONCLUIDO");
    break;
  }
  
}

void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop()
{
 leitura_ir();
 maquina_estados();
 delay(2000);
}
