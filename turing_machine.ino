/*
  Faculdade Osman Lins - FACOL - 07/12/2015
  Professor Emmanuel Barreto - Teoria da Computação
  Simulação de uma Máquina de Turing no Arduíno
  Turma do 2° Período - Sistemas de Informação
*/
#include <LiquidCrystal.h>
#include <Keypad.h>

// Estrutura da transição
struct Transicao {
  String estado;
  char simbolo;
  String novo_estado;
  char novo_simbolo;
  int direcao;
};
typedef struct Transicao transicao;

String qtd_estados = "";// Guarda a quantidade de estados da máquina

char key;// Guarda a tecla digitada

String cadeia = "";// Guarda a cadeia digitada
String estado_atual = "q0";// Guarda o estado atual
String estados_finais = "";// Guarda os estados finais

int contador=0;// Vai ser utilizada no FOR principal

const byte ROWS = 4;// Linhas do Teclado
const byte COLS = 3;// Colunas do Teclado
//Dfinindo os simbolos do Teclado
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {4, 3, 14, 2};// Configuração das Linhas do teclado
byte colPins[COLS] = {7, 6, 5};// Configuração das Colunas do teclado

Keypad teclado = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);// Configurando o teclado
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);// Configuração do LCD

// Criando um novo Simbolo (Seta UP - Representa o CABEÇOTE)
byte seta[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};
// Criando um novo Simbolo (Delta - Representa as TRANSIÇÕES)
byte delta[8] = {
  B11110,
  B11001,
  B01100,
  B01110,
  B10011,
  B10001,
  B11011,
  B01110
};
void setup() {// Função de Configuração do Arduino
  lcd.createChar(5, seta);// Criando um novo Simbolo (Seta - Cabeçote)
  lcd.createChar(6, delta);// Criando um novo Simbolo (Delta - Transições)
  lcd.begin(13, 8);// Iniciando LCD
}

void loop() {
  key = teclado.getKey();// Lendo tecla digitada

  // Apresentação de Informações
  lcd.clear();// Limpando LCD
  lcd.setCursor(0,0);
  lcd.print(" TURING MACHINE");
  lcd.setCursor(0,1);
  lcd.print("   FACOL - SI");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("RODANDO PROGRAMA");
  lcd.setCursor(0,1);
  lcd.print("   CODIGO 002");
  
while(true){
  key = teclado.getKey();
  if (key){

    Inicio:// Marcação para voltar ao inicio
    
    lcd.clear();// Limpando LCD
    lcd.setCursor(0,0);
    lcd.print("AGORA VOCE PODE");
    lcd.setCursor(0,1);
    lcd.print("  PROGRAMA-LA!");

    waitType();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("QUANTOS ESTADOS:");
    lcd.setCursor(0,1);

    while(true){
      key = teclado.getKey();
      if(key){
        if(key != '#' & key != '*'){
          qtd_estados += key;
          lcd.print(key);
        }
        if(key == '#' & qtd_estados.length() > 0){
          break;
        }
      }
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" ESTADO INICIAL:");
    
    waitType();

    //Definindo Estado Inicial
    for(int i=0; i<qtd_estados.toInt(); i++){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("       q");
      lcd.print(i);
      lcd.setCursor(0,1);
      lcd.print("* SIM      # NAO");
      while(true){
        key = teclado.getKey();
        if(key){
          if(key == '#'){
            break;// Proximo estado
          }
          if(key == '*'){
            estado_atual = "q";
            estado_atual += i;
            break;
          }
        }
      }
      if(key == '*'){
        break;
      }
    }

    //Definindo Estados Finais
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ESTADOS FINAIS:");

    waitType();
    
    for(int i=0; i<qtd_estados.toInt(); i++){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("       q");
      lcd.print(i);
      lcd.setCursor(0,1);
      lcd.print("* SIM      # NAO");
      while(true){
        key = teclado.getKey();
        if(key){
          if(key == '#'){
            break;// Proximo estado
          }
          if(key == '*'){
            estados_finais += "q";
            estados_finais += i;
            estados_finais += ";";
            break;
          }
        }
      }
    }

    transicao trs[qtd_estados.toInt() * 3];// Criando vetor de transições
    
    int pos,direcao,estadu,simbol,count=0;// Variaveis auxiliares

    // Configurando TRANSIÇÕES
    for(int i=0; i < qtd_estados.toInt(); i++){
      for(int j=0; j < 3; j++){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(6);
        lcd.print("(q");
        lcd.print(i);
        lcd.print(",");
        if(j==2){
          lcd.print("X");
        } else {
          lcd.print(j);
        }
        lcd.print(")=(q0,X,D)");
        pos = 10;
        direcao = 0;
        estadu = 0;
        simbol = 2;
      
        lcd.setCursor(pos, 1);
        lcd.write(5);
        while(true){
          key = teclado.getKey();
          if(key){
           if(key=='6' & pos < 14){
              lcd.setCursor(pos, 1);
              lcd.print(" ");
              pos += 2;
              lcd.setCursor(pos, 1);
              lcd.write(5);
           }
           if(key=='4' & pos > 10){
              lcd.setCursor(pos, 1);
              lcd.print(" ");
              pos -= 2;
              lcd.setCursor(pos, 1);
              lcd.write(5);
           }
           if(key=='2'){
              if(pos == 10){
                if(estadu == qtd_estados.toInt()-1){
                  estadu = 0;
                } else {
                  estadu++;
                }
                lcd.setCursor(10, 0);
                lcd.print(estadu);
              }
              if(pos == 12){
                if(simbol == 2){
                  simbol = 0;
                } else {
                  simbol++;
                }
                lcd.setCursor(12, 0);
                if(simbol==2){
                  lcd.print("X");
                } else {
                  lcd.print(simbol);
                }
              }
              if(pos == 14){
                lcd.setCursor(14, 0);
                if(direcao == 0){
                  direcao = -2;
                }else{
                  direcao = 0;
                }
                if(direcao == 0){
                  lcd.print("D");
                }else{
                  lcd.print("E");
                }
              }
           }
           if(key=='8'){
              if(pos == 10){
                if(estadu == 0){
                  estadu = qtd_estados.toInt()-1;
                } else {
                  estadu--;
                }
                lcd.setCursor(10, 0);
                lcd.print(estadu);
              }
              if(pos == 12){
                if(simbol == 0){
                  simbol = 2;
                } else {
                  simbol--;
                }
                lcd.setCursor(12, 0);
                if(simbol==2){
                  lcd.print("X");
                } else {
                  lcd.print(simbol);
                }
              }
              if(pos == 14){
                lcd.setCursor(14, 0);
                if(direcao == -2){
                  direcao = 0;
                }else{
                  direcao = -2;
                }
                if(direcao == 0){
                  lcd.print("D");
                }else{
                  lcd.print("E");
                }
              }
           }
           if(key=='#'){
              break;
           }
          }
        }
        // Salvando-as no vetor
        trs[count].estado = "q";
        trs[count].estado += i;
        if(j==2){
          trs[count].simbolo = 'X';
        }else if(j==1){
          trs[count].simbolo = '1';
        }else{
          trs[count].simbolo = '0';
        }
        trs[count].novo_estado = "q";
        trs[count].novo_estado += estadu;
        
        if(simbol == 2){
          trs[count].novo_simbolo = 'X';
        } else if(simbol==1){
          trs[count].novo_simbolo = '1';
        }else{
          trs[count].novo_simbolo = '0';
        }
        trs[count].direcao = direcao;
        count++;
      }
    }

    DigitarCadeia:// Marcação para voltar depois

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DIGITE A CADEIA:");
    lcd.setCursor(0,1);
  
    while(true){
      key = teclado.getKey();
      if(key){
        if (cadeia.length() == 12 & key != '#'){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ESSE E O LIMITE!");
          lcd.setCursor(0,1);
          lcd.print(" (12 SIMBOLOS)");
          delay(1500);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("DIGITE A CADEIA:");
          lcd.setCursor(0,1);
          lcd.print(cadeia);
        }
        if ((key == '1' | key == '0') & cadeia.length() < 12){
          lcd.print(key);
          cadeia += key;
          }
          if (key == '#' & cadeia.length() > 0){
            lcd.clear();
            for (contador=1; contador <= cadeia.length(); contador++){// Percorre a Cadeia digitada
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(">");
              lcd.print(cadeia);
              lcd.setCursor(14,0);
              lcd.print(estado_atual);
              lcd.setCursor(contador,1);
              lcd.write(5);    
              waitType();
              realizarTransicao(trs);// Chama a função para fazer a transição      
            }// Final do FOR
            for(int i=0; i<3; i++){
              lcd.clear();
              delay(300);
              lcd.setCursor(0,0);
              if(estados_finais.indexOf(estado_atual) != -1){// Verifica se a CADEIA foi aceita
                lcd.print(" CADEIA ACEITA");
              } else {
                lcd.print("CADEIA RECUSADA!");
              }
              lcd.setCursor(0,1);
              lcd.print("      (");
              lcd.print(estado_atual);
              lcd.print(")");
              delay(300);
            }
            cadeia = "";// Zera a cadeia
            estado_atual = "q0";// Zera o estado
            waitType();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("     MANTER");
            lcd.setCursor(0,1);
            lcd.print("  PROGRAMACAO?");
            waitType();
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("* SIM      # NAO");
            while(true){
              key = teclado.getKey();
              if(key){
                if(key == '#'){
                  qtd_estados = "";
                  estados_finais = "";
                  goto Inicio;// Volta ao inicio para programar novamente a máquina
                }
                if(key == '*'){
                  goto DigitarCadeia;// Volta para digitar uma nova CADEIA
                }
              }
            }
          }
        }
      }
    }
  }
}// Fim do loop

// Função que executa as Transições necessarias de acordo com o símbolo e o estado atuais
void realizarTransicao(transicao *trc){
  for(int i=0; i < qtd_estados.toInt() * 3; i++){//Percorre as transições até encontrar a certa
    if(estado_atual == trc[i].estado & cadeia.charAt(contador-1) == trc[i].simbolo){
      estado_atual = trc[i].novo_estado;// Troca de estado
      cadeia.setCharAt(contador-1,trc[i].novo_simbolo);// Troca o símbolo na cadeia
      contador += trc[i].direcao;// Altera o contador, fazendo o cabeçote se movimentar
      break;
    }
  }
}

// Função que apenas espera a tecla '#' ser pressionada
void waitType(){
  while(true){
    key = teclado.getKey();
    if(key){
      if(key == '#'){
        break;
      }
    }
  }
}

