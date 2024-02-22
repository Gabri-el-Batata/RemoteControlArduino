// C++ code
#include <IRremote.h>

enum EstadosCorrida {
  AGUARDANDO_CATEGORIA,
  AGUARDANDO_CALIBRACAO,
  CALIBRACAO,
  AGUARDANDO_CORRIDA,
  CORRIDA,
  CONCLUIDO
};

int RECV_PIN = 10;
EstadosCorrida estadoAtualCorr = AGUARDANDO_CATEGORIA;
bool Seguidor;

void leitura_ir() {
  if (IrReceiver.decode()){
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    //Serial.println(code, DEC); // printar o codigo ir recebido
    IrReceiver.resume(); // receber proximo valor
    
    // Secao responsavel por escolher categoria
    if ((code == 3843735296) && (estadoAtualCorr == AGUARDANDO_CATEGORIA))
    {
      // Se apertar 9 muda para o seguidor;
      Seguidor = true;
      estadoAtualCorr = AGUARDANDO_CALIBRACAO;
      Serial.println("Modo Seguidor.");
    } else if((code == 4077698816) && (estadoAtualCorr == AGUARDANDO_CATEGORIA))
    {
      	// Apertou 0 fica perseguidor
    	Seguidor = false;
      	estadoAtualCorr = CONCLUIDO;
      Serial.println("Modo Perseguidor.");
    }
    
    // Secao para parte da corrida
    if (Seguidor){
    	if ((code == 4010852096) && (estadoAtualCorr == AGUARDANDO_CALIBRACAO)){
      		// Apertou 1 utiliza calibracao ja existente
      		Serial.println("Utilizando calibracao ja existente...");
      		estadoAtualCorr = AGUARDANDO_CORRIDA;
    	}else if ((code == 3994140416) && (estadoAtualCorr == AGUARDANDO_CALIBRACAO))
    	{
      		// Apertou 2 comeca a calibrar
      		Serial.println("Calibrando...");
      		estadoAtualCorr = CALIBRACAO;
    	}
    	if ((code == 3977428736)&& (estadoAtualCorr == AGUARDANDO_CORRIDA))
    	{
      		// Apertou 3 comeca a correr
      		estadoAtualCorr = CORRIDA;
    	}
    }
    
  }
}

void maquina_estados() {
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
