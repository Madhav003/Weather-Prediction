#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store weather data
typedef struct {
    int day;
    float temperature;
} WeatherData;

// Function to read CSV file and load weather data
int read_csv(const char *filename, WeatherData *data, int max_records) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("\nError: Could not open file %s\n", filename); //!file means file not found basically
        return -1; // it basically means failure 
    }
    int count = 0;
    char line[100];
    fgets(line, sizeof(line), file); // Skip the header line
    while (fgets(line, sizeof(line), file) && count < max_records) {
        sscanf(line, "%d,%f", &data[count].day, &data[count].temperature);
        count++;
    }
    fclose(file);
    return count;
}

// Function to predict temperature
float predict(const WeatherData *data, int count, int future_day) {
    if (count < 2) {
        printf("Not enough data to make a prediction.\n");
        return 0;
    }

    // Calculate the average daily temperature change (or mean)
    float total_change = 0;
    for (int i = 1; i < count; i++) {
        total_change += data[i].temperature - data[i - 1].temperature;
    }
    float avg_change = total_change / (count - 1);

    // Predict future temperature
    int last_day = data[count - 1].day;
    float last_temp = data[count - 1].temperature;
    return last_temp + avg_change * (future_day - last_day); // predicted temperature = last temperature + (average daily change * days to predict)
}

// Function to save prediction to CSV
void save(const char *filename, int future_day, float prediction) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }
    fprintf(file, "day,predicted_temperature\n");
    fprintf(file, "%d,%.2f\n", future_day, prediction);
    fclose(file);
    printf("Prediction saved to %s\n", filename);
}

// Main function
int main() {
    WeatherData data[200];
    int count = 0, future_day;
    char input_file[200], output_file[200];
    float prediction;

    while (1) { // it is in an infinite loop so whenever there is failure, the main menu reappears
        printf("\nWeather Prediction Menu\n");
        printf("1.Load Weather Data\n");
        printf("2.Predict Temperature\n");
        printf("3.Save Prediction\n");
        printf("4.Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1: //loading data
                printf("Enter the input CSV file path: ");
                scanf("%s", input_file);
                count = read_csv(input_file, data, 100);
                if (count > 0) {
                    printf("Successfully loaded %d records from %s\n", count, input_file);
                }
                break;

            case 2: //prediction
                if (count < 1) {
                    printf("No data loaded. Please load data first.\n");
                    break;
                }
                printf("Enter the day number to predict temperature for: ");
                scanf("%d", &future_day);
                int display = future_day;
                future_day = future_day + count; // to get predection of afther the last day
                prediction = predict(data, count, future_day);
                printf("Predicted temperature for day %d: %.2f°C\n", display, prediction);
                break;

            case 3: //saving prediction
                if (prediction == 0) {
                    printf("No prediction available. Please make a prediction first.\n");
                    break;
                }
                printf("Enter the output CSV file path: ");
                scanf("%s", output_file);
                save(output_file, future_day, prediction);
                break;

            case 4: //exit
                printf("Exiting the program.\n");
                exit(1);
                return 0; //ends the program

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    return 0;
}