#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

FILE *file;

typedef struct {
    int id;
    char name[100];
    float balance;
    int age;
} User;

int verify_id() {
    file = fopen("data.txt", "r");
    
    if (fgetc(file) != EOF) {
        int max_id;
        char row[150];
        char last_row[150];
    
        while (fgets(row, sizeof(row), file) != NULL) {
            strcpy(last_row, row);
        }

        char* str_id = strtok(last_row, "|");
        max_id = atoi(str_id);
        max_id += 1;
        
        return max_id;
    } else {
        return 0;
    }
}

void create_user(User user) {
    file = fopen("data.txt", "a");

    fprintf(file, "%d|%s|%d|%.02f\n", user.id, user.name, user.age, user.balance);
    printf("\nUsuario criado com sucesso\n");
    fclose(file);
}


User read_user(int id) {
    file = fopen("data.txt", "r");
    char row[150];
    
    while (fgets(row, sizeof(row), file) != NULL) {
        User user;
        sscanf(row, "%d|%[^|]|%d|%f", &user.id, user.name, &user.age, &user.balance);
        
        if (user.id == id) {
            fclose(file);
            return user;
        }
    }
    
    fclose(file);
    User empty_user = {-1, "", 0, 0}; // Usuário vazio para indicar que não foi encontrado
    return empty_user;
}


void transfer_balance(int source_id, int destiny_id, float amount) {
    file = fopen("data.txt", "r");
    FILE *temp_file = fopen("temp_data.txt", "w");
    char row[150];
    
    while (fgets(row, sizeof(row), file) != NULL) {
        User user;
        sscanf(row, "%d|%[^|]|%d|%f", &user.id, user.name, &user.age, &user.balance);
        
        if (user.id == source_id) {
            if (amount > user.balance) {
                printf("\nErro na transferência: saldo insuficiente\n");
                fclose(file);
                fclose(temp_file);
                remove("temp_data.txt");
                return;
            }
            user.balance -= amount;
        } else if (user.id == destiny_id) {
            user.balance += amount;
        } 
        
        fprintf(temp_file, "%d|%s|%d|%.02f\n", user.id, user.name, user.age, user.balance);
    }
    
    fclose(file);
    fclose(temp_file);
    
    remove("data.txt");
    rename("temp_data.txt", "data.txt");
    
    printf("\nSaldo transferido com sucesso\n");
}


void delete_user(int id) {
    file = fopen("data.txt", "r");
    FILE *temp_file = fopen("temp_data.txt", "w");
    char row[150];
    
    while (fgets(row, sizeof(row), file) != NULL) {
        User user;
        sscanf(row, "%d|%[^|]|%d|%f", &user.id, user.name, &user.age, &user.balance);
        
        if (user.id != id) {
            fprintf(temp_file, "%d|%s|%d|%.02f\n", user.id, user.name, user.age, user.balance);
        }
    }  
    
    fclose(file);
    fclose(temp_file);
    
    remove("data.txt");
    rename("temp_data.txt", "data.txt");
    
    printf("\nUsuario deletado com sucesso\n");
}

void start_menu(int key) {
    switch (key) {
        case 1: // adicionar usuario
            User user;
            
            int id = verify_id();

            user.id = id;
            char name[100];
            
            printf("\nNome:");
            scanf("%s", name);
            
            // Verificar se o nome contém apenas letras
            int i = 0;
            while (name[i] != '\0') {
                 if (!isalpha(name[i])) {
                    printf("Nome inválido: deve conter apenas letras.\n");
                    break;
                 }
                i++;
             }
    
            if (name[i] != '\0') {
                // Se o nome contiver caracteres não alfabéticos, interromper a criação do usuário
                break;
            }
            
            strcpy(user.name, name);
            printf("Idade:");
            scanf("%d", &user.age);
            
            if (user.age < 0){
                printf ("\nIdade Inválida");
                break;
            }else {
                
                printf("Saldo:");
                scanf("%f", &user.balance);
                
                    if (user.balance < 0){
                       key = 0;
                        printf ("\nSaldo Inválido");
                        break; 
                    }
                } 
            create_user(user);
            
            
        break;
        case 2: //adicionar multiplos usuarios
            int qnt;
            
            
            
            printf("\nQuantidade de usuários:");
            scanf("%d", &qnt);

            for ( int i = 0; i < qnt; i++){
                
                user.id = verify_id();
                
                printf("\nNome:");
                scanf("%s", name);
                
                 
                // Verificar se o nome contém apenas letras
                int i = 0;
                while (name[i] != '\0') {
                     if (!isalpha(name[i])) {
                        printf("Nome inválido: deve conter apenas letras.\n");
                        break;
                     }
                    i++;
                 }
        
                if (name[i] != '\0') {
                    // Se o nome contiver caracteres não alfabéticos, interromper a criação do usuário
                    break;
                }
               
                strcpy(user.name, name);
                printf("Idade:");
                scanf("%d", &user.age);
                
                if (user.age < 0){
                    printf ("\nIdade Inválida");
                    break;
                }else {
                    
                    printf("Saldo:");
                    scanf("%f", &user.balance);
                    
                        if (user.balance < 0){
                           key = 0;
                            printf ("\nSaldo Inválido");
                            break; 
                        }
                    } 
                create_user(user);
            }
            
        break; 
          
        case 3: // buscar id
            int search_id;
            printf("Digite o ID do usuário: ");
            scanf("%d", &search_id);
            
            User found_user = read_user(search_id);
            
            if (found_user.id != -1) {
                printf("ID: %d\n", found_user.id);
                printf("Nome: %s\n", found_user.name);
                printf("Idade: %d\n", found_user.age);
                printf("Saldo: %.02f\n", found_user.balance);
            } else {
                printf("Usuário não encontrado\n");
            }
            
        break; 
        
        case 4: // transferir saldo
        
            int source_id, destiny_id;
            float amount;
            printf("Digite o ID do usuário de origem: ");
            scanf("%d", &source_id);
            
            User review_user = read_user(source_id);
            
            if (review_user.id != source_id) {
                printf("ID inválido");
                break;
            }else{
                    printf("Digite o ID do usuário de destino: ");
                    scanf("%d", &destiny_id);
                    
                    User review_user = read_user(destiny_id);
                    if (review_user.id != destiny_id) {
                        printf("ID inválido");
                         break;
                    }else{
                        printf("Digite o valor a ser transferido: ");
                        scanf("%f", &amount);
                
                         transfer_balance(source_id, destiny_id, amount);
                        }
                }
                
            
        break;  
        
        case 5: // deletar usuario
        
        int delete_id;
            printf("Digite o ID do usuário: ");
            scanf("%d", &delete_id);
            
            delete_user(delete_id);
            
        break; 
        default:
        return;
    }
        
}

int main() {
    file = fopen("data.txt", "a");
    if (file == NULL) {
        exit(1);
    }
    
    
    int key;
    
    printf("\n-- Sistema de Cadastro de Usuários --\n");
    printf("1 - Cadastrar novo usuário\n");
    printf("2 - Cadastrar múltiplos usuários\n");
    printf("3 - Buscar usuário por ID\n");
    printf("4 - Transferência de saldo\n");
    printf("5 - Excluir usuário\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &key);
       
       
    if (key == 0) {
    return 0;
    
    } else {
    start_menu(key);
    }
    
}



