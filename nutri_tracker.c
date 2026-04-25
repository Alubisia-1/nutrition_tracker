#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ENTRIES 100

struct Alias {
  char alias[50];
  char canonical[50];
};

struct Alias food_aliases[MAX_ENTRIES];
//int food_aliases_count = 0;
struct Alias unit_aliases[MAX_ENTRIES];
//int unit_aliases_count = 0;

int food_aliases_count = sizeof(food_aliases)/ sizeof(food_aliases[0]); // returns no of items

void remove_newLine(char *str) {
  str[strcspn(str, "\n")] = '\0';
}

void trim_spaces(char *str) {
  int start = 0;
  int end = strlen(str) - 1;
  int i,j;

  while (str[start] == ' ' || str[start] == '\t') {
    start++;
  };

  while(end >= start && (str[end] == ' ' || str[end] == '\t')) {
    end--;
  };
  
  for (i = start, j=0; i <= end; i++, j++) {
    str[j] = str[i];
  };
  str[j] = '\0';
}

void to_lowercase(char *str) {
  for(int i = 0; str[i] != '\0'; i++) {
    str[i] = tolower((unsigned char)str[i]);
  }
}

int read_aliases(char filename[], struct Alias aliases[], int *aliasCount) {
  FILE *file = fopen(filename, "r");
  char line[100];

  *aliasCount = 0;

  if(!file) {
    printf("Failed to read file: %s\n", filename);
    return 0;
  }

  while(fgets(line, sizeof(line), file)) {
      remove_newLine(line);

      char *alias = strtok (line, ",");
      char *canonical = strtok (NULL, ",");

      if(alias && canonical) {
        trim_spaces(alias);
        trim_spaces(canonical);
        to_lowercase(alias);
        to_lowercase(canonical);

        strcpy(aliases[*aliasCount].alias, alias);
        strcpy(aliases[*aliasCount].canonical, canonical);

        (*aliasCount)++;
        if(*aliasCount >= MAX_ENTRIES) break;
      }
    }
  if(*aliasCount == 0) {
    printf("No entries in file: %s\n", filename);
    return 0;
  }
  return 1;
}

void normalize_text(char *str) {
  remove_newLine(str);
  trim_spaces(str);
  to_lowercase(str);
}

void canonicalizer(char *str, struct Alias aliases[], int count) {
  for(int i = 0; i < count; i++) {
    if(strcmp(str, aliases[i].alias) == 0) {
      strcpy(str, aliases[i].canonical);
      return;
    }
  }
}

struct Alias unit_aliases[] = {
  {"kg", "kg"},
  {"kilogram", "kg"},
  {"kgs", "kg"},

  {"ml", "ml"},
  {"mililitres", "ml"},

  {"l", "l"},
  {"litres", "l"},

  {"pcs", "piece"},
  {"pieces", "piece"},
  {"piece", "piece"},

  {"cup", "cup"},
  {"cups", "cup"},

  {"g", "g"},
  {"grams", "g"},
  {"gram", "g"}
};

int unit_aliases_count = sizeof(unit_aliases)/ sizeof(unit_aliases[0]);


struct Nutrition {
  char name[50];
  float protein;
  float sugar;
  float iron;
  char user_unit[10];
  float ref_unit;
};

struct Nutrition nutrition_db[MAX_ENTRIES];
int nutrition_count = 0;

void load_nutrition_data() {
  strcpy(nutrition_db[0].name, "rice");
  nutrition_db[0].protein = 2.7;
  nutrition_db[0].sugar = 0.1;
  nutrition_db[0].iron = 1.2;
  strcpy(nutrition_db[0].user_unit, "g");
  nutrition_db[0].ref_unit = 100;
  nutrition_count++;

  strcpy(nutrition_db[1].name, "eggs");
  nutrition_db[1].protein = 13;
  nutrition_db[1].sugar = 1.1;
  nutrition_db[1].iron = 1.8;
  strcpy(nutrition_db[1].user_unit, "pcs");
  nutrition_db[1].ref_unit = 1;
  nutrition_count++;

  strcpy(nutrition_db[2].name, "milk");
  nutrition_db[2].protein = 3.4;
  nutrition_db[2].sugar = 5;
  nutrition_db[2].iron = 0.03;
  strcpy(nutrition_db[2].user_unit, "ml");
  nutrition_db[2].ref_unit = 100;
  nutrition_count++;
}

void show_nutrition_db() {
  for(int i = 0; i < nutrition_count; i++) {
    printf("%s Protein: %.2f Sugar: %.2f Iron: %.2f\n",
          nutrition_db[i].name,
          nutrition_db[i].protein,
          nutrition_db[i].sugar,
          nutrition_db[i].iron
        );
  }
}

void display_menu() {
    printf("=== Nutrion Tracker ===\n"
          "1. Add Food Entry\n"
          "2. View Entries\n"
          "3. View Nutrions Values for current entries\n"
          "4. Calculate Nutrition\n"
          "5. EXit\n"
      );
}

int get_choice() {
  int choice;
  printf("Chose a Menu: 1,2,3\n");
  scanf("%d", &choice);
  getchar();
  return choice;
}

struct FoodEntry {
  char name[50];
  float quantity;
  char unit[10]; // "ml" "g" "mg"
};

struct FoodEntry food[MAX_ENTRIES];
int count = 0;

int add_food(struct FoodEntry entry) {
  FILE *file = fopen("Food.txt", "a");

  if(!file) {
    printf("Failed to write to file: Food.txt\n");
    return 0;
  }

  fprintf(file, "%s %.2f %s\n",
        entry.name,
        entry.quantity,
        entry.unit
      );
  fclose(file);
  return 1;
}

int read_food() {
  FILE *file = fopen("Food.txt", "r");
  count = 0;

  if(!file) {
    printf("Failed to read file: Food.txt\n");
    return 0;
  }

  while(fscanf(file, "%s %f %s",
          food[count].name,
          &food[count].quantity,
          food[count].unit
        ) == 3) {
    count++;
    if(count >= MAX_ENTRIES) break;
  }
  fclose(file);

  if(count == 0) {
    printf("No entries in the file: Food.txt\n");
    return 0;
  }
    return 1;
}

void food_entry() {
  if(count >= MAX_ENTRIES) {
    printf("Memory is Full!\n");
    return;
  }
  printf("Enter Food/Snack already eaten: ");
  fgets(food[count].name, sizeof(food[count].name), stdin);
  normalize_text(food[count].name);
  canonicalizer(food[count].name, food_aliases, food_aliases_count);

  printf("Enter quantity: ");
  scanf("%f", &food[count].quantity);
  while(getchar() != '\n');

  printf("Enter unit (mg/g/pcs): ");
  fgets(food[count].unit, sizeof(food[count].unit), stdin);
  normalize_text(food[count].unit);
  canonicalizer(food[count].unit, unit_aliases, unit_aliases_count);

  if(add_food(food[count])) {
    printf("Food succesfully added\n");
    printf("Name: %s\n", food[count].name);
  } else {
    printf("Failed to add Food\n");
  };
  count++;
}

void view_entries() {
  if (!read_food()) return;

  for(int i = 0; i < count; i++) {
    printf("%d. %s %.2f %s\n",
      i + 1,
      food[i].name,
      food[i].quantity,
      food[i].unit
    );
  }
}

int find_nutrition(char food_name[]) {
  for(int i = 0; i < nutrition_count; i++) {
    if(strcmp(food_name, nutrition_db[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

void test_search() {
 if(!read_food()) return;
  for(int i = 0; i < count; i++) {
    int index = find_nutrition(food[i].name);
    if(index != -1) {
    printf("%s Protein: %.2f Sugar: %.2f Iron: %.2f\n",
            nutrition_db[index].name,
            nutrition_db[index].protein,
            nutrition_db[index].sugar,
            nutrition_db[index].iron
          );
    } else {
      printf("Food Not Found in Nutrition Database!\n");
    } 
  }
}

float convert_to_ref_unit(char food_name[], float quantity, char unit[]) {
  if (strcmp(food_name, "rice") == 0) {
    if(strcmp(unit, "g") == 0) return quantity;
    if(strcmp(unit, "kg") == 0) return quantity * 1000;
    if(strcmp(unit, "plate") == 0 || strcmp(unit, "plates") == 0) return quantity* 250;
    if(strcmp(unit, "cup") == 0 || strcmp(unit, "cups") == 0) return quantity * 195;
  }

  if(strcmp(food_name, "milk") == 0) {
    if(strcmp(unit, "ml") == 0) return quantity;
    if(strcmp(unit, "l") == 0) return quantity * 1000;
    if(strcmp(unit, "cup") == 0 || strcmp(unit, "cups") == 0) return quantity * 240;
  }

  if(strcmp(food_name, "eggs") == 0){
    if(strcmp(unit, "pieces") == 0 || strcmp(unit, "pcs") == 0 || strcmp(unit, "piece") == 0) return quantity;
  }

  return -1;
}

void calculate_nutrition() {
  if(!read_food()) return;
  for(int i = 0; i < count; i++) {
    int index = find_nutrition(food[i].name);
    if(index != -1) {

      float converted_quantity = convert_to_ref_unit(
            food[i].name,
            food[i].quantity,
            food[i].unit
          );

      if(converted_quantity == -1) {
        printf("%s: Unsupported quantity '%s'\n", food[i].name, food[i].unit);
        continue;
      }

      //if(strcmp(food[i].unit, nutrition_db[index].user_unit) == 0) { 
        float multiplier = converted_quantity / nutrition_db[index].ref_unit;
        float actual_protein = nutrition_db[index].protein * multiplier;
        float actual_sugar = nutrition_db[index].sugar * multiplier;
        float actual_iron = nutrition_db[index].iron * multiplier;

        printf("%s Protein: %.2f Sugar: %.2f Iron: %.2f\n",
              nutrition_db[index].name,
              actual_protein,
              actual_sugar,
              actual_iron
            );
    /*
      } else {
       printf("Units Do Not Match!\n");
      }
    */
      }  else {
        printf("Food Not Found!"); 
     }
    } 
  }

int main() {
  int choice;

  read_aliases("food_aliases.txt", food_aliases, &food_aliases_count);
  read_aliases("unit_aliases.txt", unit_aliases, &unit_aliases_count);
  load_nutrition_data();

  while (1) {
    display_menu();

    choice = get_choice();
  
   if(choice == 1) {
     food_entry();
   } else if (choice == 2) {
     view_entries();
   } else if(choice == 3) {
     test_search(); 
   } else if (choice == 4) {
     calculate_nutrition();
   } else if (choice == 5) {
     printf("Exiting...!\n");
     break;
   } else {
     printf("Invalid Choice!");
   }
  } 
  return 0;
}
