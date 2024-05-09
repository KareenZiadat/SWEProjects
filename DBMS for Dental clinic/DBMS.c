#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <pthread.h>

struct Patient{
    int Patient_ID;
    char First_Name[11];
    char Last_Name[21];
    char Email[151];
    char Phone_Number[11];
    struct Patient* next;
} ;
struct Dentist{
    int Dentist_ID;
    char First_Name[10];
    char Last_Name[20];
    char Email[150];
    char Speciality[100];
    char Phone_Number[10];
    struct Dentist* next;
};
struct Date{
    int id;
    int day;
    int month;
    int year;
    struct Date* next;
};
struct Time{
    int id;
    int hours;
    int minutes;
    struct Time* next;
};
struct Appointment{
    int Appointment_ID;
    int Patient_ID;
    int Dentist_ID;
    int Treatment_ID;
    struct Date appointment_date;
    struct Time appointment_time;
    char status[20];
    struct Appointment* next;
};
struct Treatment{
    int Treatment_ID;
    char Treatment_name[10];
    int price;
    char Description[100];
    struct Treatment* next;
};
// Define a helper structure to pass node information to the thread function
struct PrintData {
    struct Appointment* node;
};
// Define a helper structure to pass dentist information to the thread function
struct PrintDentist {
    struct Dentist* dentist;
};
// Define a helper structure to pass treatment information to the thread function
struct PrintTreatment {
    struct Treatment* treatment;
};

// view schedule
void viewSchedule(struct Appointment* appointments, struct Patient* patients, struct Dentist* dentists, struct Treatment* treatments) {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);
    int today_day = local_time->tm_mday;
    int today_month = local_time->tm_mon + 1;
    int today_year = local_time->tm_year + 1900;

    printf("Viewing today's schedule...\n");
    printf("Appointment ID\tPatient Name\t\tDentist Name\t\tTreatment Name\t\tDate\t\tTime\t\tStatus\n");

    struct Appointment* current = appointments;
    while (current != NULL) {
        if (current->appointment_date.day == today_day &&
            current->appointment_date.month == today_month &&
            current->appointment_date.year == today_year) {
            // Find patient by ID
            struct Patient* patient = patients;
            while (patient != NULL) {
                if (patient->Patient_ID == current->Patient_ID) {
                    break;
                }
                patient = patient->next;
            }

            // Find dentist by ID
            struct Dentist* dentist = dentists;
            if(current->Dentist_ID==-1){
                snprintf(dentist->First_Name, sizeof(dentist->First_Name), "%s", "unknown");
                snprintf(dentist->Last_Name, sizeof(dentist->Last_Name), "%s", "");
            }
            else{
            while (dentist != NULL) {
                if (dentist->Dentist_ID == current->Dentist_ID) {
                    break;
                }
                dentist = dentist->next;
            }
            }

            // Find treatment by ID
            struct Treatment* treatment = treatments;
            if(current->Treatment_ID==-1){
                snprintf(treatment->Treatment_name, sizeof(treatment->Treatment_name), "%s", "unknown");
            }
            else{
            while (treatment != NULL) {
                if (treatment->Treatment_ID == current->Treatment_ID) {
                    break;
                }
                treatment = treatment->next;
            }
            }

            if (patient != NULL && dentist != NULL && treatment != NULL) {
                printf("%d\t\t%s %s\t\t%s %s\t%s\t\t%d/%d/%d\t%02d:%02d\t\t%s\n",
                       current->Appointment_ID,
                       patient->First_Name,
                       patient->Last_Name,
                       dentist->First_Name,
                       dentist->Last_Name,
                       treatment->Treatment_name,
                       current->appointment_date.day,
                       current->appointment_date.month,
                       current->appointment_date.year,
                       current->appointment_time.hours,
                       current->appointment_time.minutes,
                       current->status);
            }
        }
        current = current->next;
    }
}

// Function to find a patient by ID in the patients linked list by implementing the Binary Search on linked list
struct Patient* middle(struct Patient* head)
{
    if (head == NULL) {
        return NULL; // Empty linked list
    }

    struct Patient* slow = head;
    struct Patient* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
struct Patient* findPatientByID(int patient_id,struct Patient* head)
{
    struct Patient* start = head;
    struct Patient* last = NULL;

    do {
        // Find middle
        struct Patient* mid = middle(start);

        // If middle is empty
        if (mid == NULL)
            return NULL;

        // If value is present at middle
        if (mid->Patient_ID == patient_id)
            return mid;

        // If value is more than mid
        else if (mid->Patient_ID < patient_id)
            start = mid->next;

        // If the value is less than mid.
        else
            last = mid;

    } while (last == NULL || last != start);

    // Patient not found
    return NULL;
}


// Function to find a dentist by ID in the dentists linked list by implementing the Binary Search on linked list
struct Dentist* dentistMiddle(struct Dentist* head)
{
    if (head == NULL) {
        return NULL; // Empty linked list
    }

    struct Dentist* slow = head;
    struct Dentist* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
struct Dentist* findDentistByID(int dentist_id,struct Dentist* head)
{
    struct Dentist* start = head;
    struct Dentist* last = NULL;

    do {
        // Find middle
        struct Dentist* mid = dentistMiddle(start);

        // If middle is empty
        if (mid == NULL)
            return NULL;

        // If value is present at middle
        if (mid->Dentist_ID == dentist_id)
            return mid;

        // If value is more than mid
        else if (mid->Dentist_ID < dentist_id)
            start = mid->next;

        // If the value is less than mid.
        else
            last = mid;

    } while (last == NULL || last != start);

    // dentist not found
    return NULL;
}


// Function to find a treatment by ID in the treatments linked list by implementing the Binary Search on linked list
struct Treatment* treatmentMiddle(struct Treatment* head)
{
    if (head == NULL) {
        return NULL; // Empty linked list
    }

    struct Treatment* slow = head;
    struct Treatment* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
struct Treatment* findTreatmentByID(int treatment_id,struct Treatment* head)
{
    struct Treatment* start = head;
    struct Treatment* last = NULL;

    do {
        // Find middle
        struct Treatment* mid = treatmentMiddle(start);

        // If middle is empty
        if (mid == NULL)
            return NULL;

        // If value is present at middle
        if (mid->Treatment_ID == treatment_id)
            return mid;

        // If value is more than mid
        else if (mid->Treatment_ID < treatment_id)
            start = mid->next;

        // If the value is less than mid.
        else
            last = mid;

    } while (last == NULL || last != start);

    // Treatment not found
    return NULL;
}

// Function to find an appointment by ID in the appointments linked list by implementing Binary Search on linked list
struct Appointment* appointmentMiddle(struct Appointment* head)
{
    if (head == NULL) {
        return NULL; // Empty linked list
    }

    struct Appointment* slow = head;
    struct Appointment* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
struct Appointment* findAppointmentByID(int appointment_id, struct Appointment* head)
{
    struct Appointment* start = head;
    struct Appointment* last = NULL;

    do {
        // Find middle
        struct Appointment* mid = appointmentMiddle(start);

        // If middle is empty
        if (mid == NULL)
            return NULL;

        // If value is present at middle
        if (mid->Appointment_ID == appointment_id)
            return mid;

        // If value is more than mid
        else if (mid->Appointment_ID < appointment_id)
            start = mid->next;

        // If the value is less than mid.
        else
            last = mid;

    } while (last == NULL || last != start);

    // Appointment not found
    return NULL;
}


//function to write patient to file
void writePatient(struct Patient *h){
    FILE *fp=fopen("patients","w");
    while(h!=NULL){
        fwrite(h,sizeof(struct Patient),1,fp);
        h=h->next;
    }
    fclose(fp);
    printf("Linked List data is written to the file\n");
}
// Function to create a new Patient node and add it to the linked list
void addPatient(struct Patient** head, const char* first_name, const char* last_name, const char* email, const char* phone_number) {
    // Check if phone number already exists
    struct Patient* current = *head;
    while (current != NULL) {
        if (strcmp(current->Phone_Number, phone_number) == 0) {
            printf("Phone number already exists.\n");
            return; // Exit the function without adding the patient
        }
        current = current->next;
    }

    // Check if phone number is 10 digits
    if (strlen(phone_number) != 10) {
        printf("Phone number should be 10 digits.\n");
        return; // Exit the function without adding the patient
    }

    // Check if email already exists
    current = *head;
    while (current != NULL) {
        if (strcmp(current->Email, email) == 0) {
            printf("Email already exists.\n");
            return; // Exit the function without adding the patient
        }
        current = current->next;
    }

    // Create and add the new patient to the linked list
    struct Patient* newPatient = (struct Patient*)malloc(sizeof(struct Patient));
    snprintf(newPatient->First_Name, sizeof(newPatient->First_Name), "%s", first_name);
    snprintf(newPatient->Last_Name, sizeof(newPatient->Last_Name), "%s", last_name);
    snprintf(newPatient->Email, sizeof(newPatient->Email), "%s", email);
    snprintf(newPatient->Phone_Number, sizeof(newPatient->Phone_Number), "%s", phone_number);
    newPatient->next = NULL;

    if (*head == NULL) {
        *head = newPatient;
        newPatient->Patient_ID = 1;
    } else {
        struct Patient* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }

        // Find the last node to get the highest ID
        struct Patient* lastNode = current;
        int id = lastNode->Patient_ID + 1;

        newPatient->Patient_ID = id;
        current->next = newPatient;
    }

    // Calculate the number of elements in the linked list
    int count = 0;
    current = *head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    writePatient(*head);
    
    // Reallocate memory to accommodate the new size
    *head = realloc(*head, count * sizeof(struct Patient));
    
}

//function to write dates to file
void writeDate(struct Date *h){
    FILE *fp=fopen("dates","w");
    while(h!=NULL){
        fwrite(h,sizeof(struct Date),1,fp);
        h=h->next;
    }
    fclose(fp);
    printf("Linked List data is written to the file\n");
}
// Function to create a new Date node and add it to the linked list
struct Date* newDate(struct Date** head, int day, int month, int year) {
    struct Date* newDate = (struct Date*)malloc(sizeof(struct Date));
    newDate->day = day;
    newDate->month = month;
    newDate->year = year;
    newDate->next = NULL;

    if (*head == NULL) {
        newDate->id = 1;
        *head = newDate;
    } else {
        struct Date* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }

        // Find the last node to get the highest ID
        struct Date* lastNode = current;
        int id = lastNode->id + 1;

        newDate->id = id;
        current->next = newDate;
    }
    
    // Calculate the number of elements in the linked list
    int count = 0;
    struct Date* current = *head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Reallocate memory to accommodate the new size
    *head = realloc(*head, count * sizeof(struct Date));
    writeDate(*head);

    return newDate;
}


//function to write time to file
void writeTime(struct Time *h){
    FILE *fp=fopen("times","w");
    while(h!=NULL){
        fwrite(h,sizeof(struct Time),1,fp);
        h=h->next;
    }
    fclose(fp);
    printf("Linked List data is written to the file\n");
}
// Function to create a new time node and add it to the linked list
struct Time* newTime(struct Time** head, int hr, int min) {
    struct Time* newTime = (struct Time*)malloc(sizeof(struct Time));
    newTime->hours = hr;
    newTime->minutes = min;
    newTime->next = NULL;

    if (*head == NULL) {
        newTime->id = 1;
        *head = newTime;
    } else {
        struct Time* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }

        // Find the last node to get the highest ID
        struct Time* lastNode = current;
        int id = lastNode->id + 1;

        newTime->id = id;
        current->next = newTime;
    }

    // Calculate the number of elements in the linked list
    int count = 0;
    struct Time* current = *head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Reallocate memory to accommodate the new size
    *head = realloc(*head, count * sizeof(struct Time));

    writeTime(*head);

    return newTime;
}

//function to write appointments to file
void writeAppointment(struct Appointment *h){
    FILE *fp=fopen("appointments","w");
    while(h!=NULL){
        fwrite(h,sizeof(struct Appointment),1,fp);
        h=h->next;
    }
    fclose(fp);
    printf("Linked List data is written to the file\n");
}
// Function to create a new Appointment node and add it to the linked list
void createAppointment(struct Appointment** head, int patient_id, int dentist_id, int treatment_id, struct Date date, struct Time time, struct Patient* patientList, struct Dentist* dentistList, struct Treatment* treatmentList) {
    // Check if patient_id exists in the patients linked list
    struct Patient* patient = findPatientByID(patient_id, patientList);
    if (patient == NULL) {
        printf("Patient with ID %d does not exist.\n", patient_id);
        return; // Exit the function without adding the appointment
    }

    // Check if dentist_id exists in the dentist linked list (if not -1)
    struct Dentist* dentist = NULL;
    if (dentist_id != -1) {
        dentist = findDentistByID(dentist_id, dentistList);
        if (dentist == NULL) {
            printf("Dentist with ID %d does not exist.\n", dentist_id);
            return; // Exit the function without adding the appointment
        }
    }

    // Check if treatment_id exists in the treatments linked list (if not -1)
    struct Treatment* treatment = NULL;
    if (treatment_id != -1) {
        treatment = findTreatmentByID(treatment_id, treatmentList);
        if (treatment == NULL) {
            printf("Treatment with ID %d does not exist.\n", treatment_id);
            return; // Exit the function without adding the appointment
        }
    }

    // Check if appointment already exists for the given date and time
    struct Appointment* current = *head;
    while (current != NULL) {
        if (current->appointment_date.day == date.day &&
            current->appointment_date.month == date.month &&
            current->appointment_date.year == date.year &&
            current->appointment_time.hours == time.hours &&
            current->appointment_time.minutes == time.minutes) {
            printf("Appointment already exists for the given date and time.\n");
            return; // Exit the function without adding the appointment
        }
        current = current->next;
    }

    // Create and add the new appointment to the linked list
    struct Appointment* newAppointment = (struct Appointment*)malloc(sizeof(struct Appointment));
    newAppointment->Patient_ID = patient_id;
    newAppointment->Dentist_ID = dentist_id;
    newAppointment->Treatment_ID = treatment_id;
    newAppointment->appointment_date = date;
    newAppointment->appointment_time = time;
    snprintf(newAppointment->status, sizeof(newAppointment->status), "%s", "Scheduled");
    newAppointment->next = NULL;

    if (*head == NULL) {
        *head = newAppointment;
        newAppointment->Appointment_ID = 1;
    } else {
        struct Appointment* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }

        // Find the last node to get the highest ID
        struct Appointment* lastNode = current;
        int id = lastNode->Appointment_ID + 1;

        newAppointment->Appointment_ID = id;
        current->next = newAppointment;
    }
    // Calculate the number of elements in the linked list
    int count = 0;
    current = *head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    
    // Reallocate memory to accommodate the new size
    *head = realloc(*head, count * sizeof(struct Appointment));
    writeAppointment(*head);

    
}
//create Appointment
void createAppointmentNewPatient(struct Patient** patients,struct Date** dates,struct Time** times,struct Appointment** appointments,struct Dentist** dentists, struct Treatment** treatments){
            int choice;
            char first_name[50];
            char last_name[50];
            char phone_number[11];
            int dentist=-1;
            int treatment=-1;
            int day;
            int month;
            int year;
            int hr;
            int min;
            struct Date* appointment_date;
            struct Time* appointment_time;
            int patientID = 0;
            printf("Creating patient ......... \n");
            printf("Enter first name: ");
            scanf("%s", first_name);
            printf("Enter last name: ");
            scanf("%s", last_name);
            printf("Enter phone number (10 digits): ");
            scanf("%s", phone_number);
            addPatient(patients, first_name, last_name, "", phone_number);
            // Find the ID of the last added patient
            struct Patient* current = *patients;
            while (current != NULL) {
                patientID = current->Patient_ID;
                current = current->next;
            }
            
            printf("Creating appointment ......... \n");
            printf("1- All appointment details are known\n");
            printf("2- The dentist of the appointment is unknown\n");
            printf("3- The treatment of the appointment is unknown\n");
            printf("4- The dentist and treatment of the appointment are unknown\n");
            printf("5- Back\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            
    
            switch (choice) {
                case 1:
                printf("Enter the dentist's ID: ");
                scanf("%d", &dentist);
                printf("Enter the treatment ID: ");
                scanf("%d", &treatment);
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);
                break;
            
                
                case 2:
                printf("Enter the treatment ID: ");
                scanf("%d", &treatment);
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);
                break;
                
                case 3:
                printf("Enter the dentist's ID: ");
                scanf("%d", &dentist);
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);
                break;
                
                case 4:
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);
                break;
                
                case 5:
                printf("Going back to the main menu.\n");
                return;
                
                default:
                printf("Invalid choice. Please try again.\n");
                break;
            }
            }
void createAppointmentPatientExists(struct Patient** patients,struct Date** dates,struct Time** times,struct Appointment** appointments,struct Dentist** dentists, struct Treatment** treatments){
            int choice;
            printf("1- All details are known\n");
            printf("2- The dentist is unknown\n");
            printf("3- The treatment is unknown\n");
            printf("4- The dentist and treatment are unknown\n");
            printf("5- Back\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            int patientID;
            int dentist=-1;
            int treatment=-1;
            int day;
            int month;
            int year;
            int hr;
            int min;
            struct Date* appointment_date;
            struct Time* appointment_time;
            switch (choice) {
                case 1:
                printf("Enter the patient's ID: ");
                scanf("%d", &patientID);
                printf("Enter the dentist's ID: ");
                scanf("%d", &dentist);
                printf("Enter the treatment ID: ");
                scanf("%d", &treatment);
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);           
                break;
               
                
                case 2:
                printf("Enter the patient's ID: ");
                scanf("%d", &patientID);
                printf("Enter the treatment ID: ");
                scanf("%d", &treatment);
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);
                break;
                
                case 3:
                printf("Enter the patient's ID: ");
                scanf("%d", &patientID);
                printf("Enter the Dentist ID: ");
                scanf("%d", &dentist);
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);
                break;
                
                case 4:
                printf("Enter the patient's ID: ");
                scanf("%d", &patientID);
                printf("Enter appointment date (DD/MM/YYYY): ");
                scanf("%d/%d/%d", &day, &month, &year);
                // Create the new date by calling newDate function
                appointment_date = newDate(dates, day, month, year);
                printf("Enter appointment time (HH:MM): ");
                scanf("%d:%d", &hr, &min);
                // Create the new time by calling newTime function
                appointment_time = newTime(times, hr, min);
                // Create the new appointment by calling createAppointment function
                createAppointment(appointments, patientID, dentist, treatment, *appointment_date, *appointment_time, *patients, *dentists, *treatments);
                break;
                
                case 5:
                printf("Going to the previous menu.\n");
                return;
                
                default:
                printf("Invalid choice. Please try again.\n");
                break;
            }
}   
void createAppointmentMain(struct Patient* patients,struct Date* dates,struct Time* times,struct Appointment* appointments,struct Dentist* dentists, struct Treatment* treatments){
    int choice;
    printf("1- This is the patient’s first appointment\n");
    printf("2- The patient is already registered\n");
    printf("3- Back\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
        createAppointmentNewPatient(&patients,&dates,&times,&appointments,&dentists, &treatments);
        break;
        case 2: 
        createAppointmentPatientExists(&patients,&dates,&times,&appointments,&dentists, &treatments);
        break;
        
        case 3:
        printf("Going back to the main menu.\n");
        return;
                
        default:
        printf("Invalid choice. Please try again.\n");
        break;
    }
}

//modify appointment  
void changeAppointmentDate(struct Appointment* appointments, int appointmentID,int patientID,int newDay, int newMonth, int newYear) {
    struct Appointment* current = appointments;
    if(appointmentID!=-1){
    current = findAppointmentByID(appointmentID, appointments);
    if (current != NULL) {
            current->appointment_date.day = newDay;
            current->appointment_date.month = newMonth;
            current->appointment_date.year = newYear;
            snprintf(current->status, sizeof(current->status), "%s", "Rescheduled");
            printf("Appointment date changed successfully.\n");
            return;
        }
    printf("Appointment ID not found.\n");
    }
    else if(patientID!=-1){
            while (current != NULL) {
                if (current->Patient_ID == patientID) {
                    current->appointment_date.day = newDay;
                    current->appointment_date.month = newMonth;
                    current->appointment_date.year = newYear;
                    printf("Appointment date changed successfully.\n");
                    return;
                }
                current = current->next;
            }
            printf("Patient ID not found.\n");
        }
}
void changeAppointmentTime(struct Appointment* appointments, int appointmentID,int patientID,int newHr, int newMin){
    struct Appointment* current = appointments;
    if(appointmentID!=-1){
    current = findAppointmentByID(appointmentID, appointments);
    if (current != NULL) {
            current->appointment_time.hours = newHr;
            current->appointment_time.minutes = newMin;
            snprintf(current->status, sizeof(current->status), "%s", "Rescheduled");
            printf("Appointment time changed successfully.\n");
            return;
    }
    printf("Appointment ID not found.\n");
    }
    
    else if(patientID!=-1){
            while (current != NULL) {
                if (current->Patient_ID == patientID) {
                current->appointment_time.hours = newHr;
                current->appointment_time.minutes = newMin;
                printf("Appointment time changed successfully.\n");
                    return;
                }
                current = current->next;
            }
            printf("Patient ID not found.\n");
        }
}
void changeAppointmentDoctor(struct Appointment* appointments, int appointmentID,int patientID,int newID){
     struct Appointment* current = appointments;
    if(appointmentID!=-1){
    current = findAppointmentByID(appointmentID, appointments);
    if (current != NULL) {
            current->Dentist_ID = newID;
            snprintf(current->status, sizeof(current->status), "%s", "Dentist Changed");
            printf("Dentist for appointment changed successfully.\n");
            return;
    }
    printf("Appointment ID not found.\n");
    }
    
    else if(patientID!=-1){
            while (current != NULL) {
                if (current->Patient_ID == patientID) {
                current->Dentist_ID = newID;
                printf("Dentist for appointment changed successfully.\n");
                return;
            }
                current = current->next;
            }
            printf("Patient ID not found.\n");
        }
}
void changeAppointmentTreatment(struct Appointment* appointments, int appointmentID,int patientID,int newID){
     struct Appointment* current = appointments;
     if(appointmentID!=-1){
    current = findAppointmentByID(appointmentID, appointments);
    if (current != NULL) {
            current->Treatment_ID = newID;
            snprintf(current->status, sizeof(current->status), "%s", "Treatment changed");
            printf("Treatment for appointment changed successfully.\n");
            return;
    }
    printf("Appointment ID not found.\n");
    }
    
    else if(patientID!=-1){
            while (current != NULL) {
            if (current->Patient_ID == patientID) {
            current->Treatment_ID = newID;
            printf("Treatment for appointment changed successfully.\n");
            return;
            }
                current = current->next;
            }
            printf("Patient ID not found.\n");
        }
}
void changeAppointmentDetails(struct Appointment* appointments, struct Dentist* dentists,struct Treatment* treatments) {
    int choice;
    do{
    printf("1- By appointment ID\n");
    printf("2- By patient ID\n");
    printf("3- Back\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    struct Date date;
    struct Time time;
    int appointmentID=0;
    int treatmentID=0;
    int doctorID=0;
    switch (choice) {
        case 1:
        int appointmentID;
        printf("Enter the appointment ID: ");
        scanf("%d", &appointmentID);
        int subchoice1;
        printf("1- Change date\n");
        printf("2- Change time\n");
        printf("3- Change doctor\n");
        printf("4- Change treatment\n");
        printf("5- Back\n");
        printf("Enter your choice: ");
        scanf("%d", &subchoice1);
        switch(subchoice1){
            case 1:
            printf("Enter the new appointment date (DD/MM/YYYY): ");
            scanf("%d/%d/%d", &date.day, &date.month, &date.year);
            changeAppointmentDate(appointments, appointmentID, -1,date.day, date.month,date.year);
            break;
            
            case 2:
            printf("Enter appointment time (HH:MM:SS): ");
            scanf("%d:%d", &time.hours, &time.minutes);
            changeAppointmentTime(appointments, appointmentID, -1,time.hours, time.minutes);
            break;
            
            case 3:
            printf("Enter new dentist ID: ");
            scanf("%d", &doctorID);
            if(findDentistByID(doctorID,dentists)!=NULL){
            changeAppointmentDoctor(appointments, appointmentID, -1,doctorID);}
            else{
                printf("Dentist Id does not exist");
            }
            break;
            
            case 4:
            printf("Enter new treatment ID: ");
            scanf("%d", &treatmentID);
            if(findTreatmentByID(treatmentID, treatments)!=NULL){
            changeAppointmentTreatment(appointments, appointmentID, -1,treatmentID);}
            else{
                printf("Treatment Id does not exist");
            }
            break;
            
            case 5:
            printf("Going back to the main menu.\n");
            return;
            
            default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
        break;
        
        case 2:
        int patientID;
        //if i want to search by patient 
        printf("Enter the patient ID: ");
        scanf("%d", &patientID);
        int subchoice2;
        printf("1- Change date\n");
        printf("2- Change time\n");
        printf("3- Change doctor\n");
        printf("4- Change treatment\n");
        printf("5- Back\n");
        printf("Enter your choice: ");
        scanf("%d", &subchoice2);
        switch(subchoice2){
            case 1:
            printf("Enter the new appointment date (DD/MM/YYYY): ");
            scanf("%d/%d/%d", &date.day, &date.month, &date.year);
            changeAppointmentDate(appointments, -1, patientID,date.day, date.month,date.year);
            break;
            
            
            case 2:
            printf("Enter appointment time (HH:MM:SS): ");
            scanf("%d:%d", &time.hours, &time.minutes);
            changeAppointmentTime(appointments, -1, patientID,time.hours, time.minutes);
            break;
            
            case 3:
            printf("Enter new dentist ID: ");
            scanf("%d", &doctorID);
            if(findDentistByID(doctorID,dentists)!=NULL){
            changeAppointmentDoctor(appointments, -1, patientID,doctorID);}
            else{
                printf("Dentist Id does not exist");
            }
            break;
            
            case 4:
            printf("Enter new treatment ID: ");
            scanf("%d", &treatmentID);
           if(findTreatmentByID(treatmentID, treatments)!=NULL){
            changeAppointmentTreatment(appointments, -1, patientID,treatmentID);}
            else{
                printf("Treatment Id does not exist");
            }
            break;
            
            case 5:
            printf("Going back to the main menu.\n");
            return;
            
            default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
        break;
        
        case 3:
        printf("Going back to the main menu.\n");
        return;

        default:
        printf("Invalid choice. Please try again.\n");
        break;
    }
        
    }
    while(choice!=3);
}

//delete appointment
// Delete appointment by ID
void deleteAppointmentByID(struct Appointment** head, int appointment_id) {
    if (*head == NULL) {
        printf("Appointment list is empty.\n");
        return;
    }

    // Find the appointment to be deleted
    struct Appointment* appointmentToDelete = findAppointmentByID(appointment_id, *head);

    if (appointmentToDelete == NULL) {
        printf("Appointment with ID %d not found.\n", appointment_id);
        return;
    }

    // Check if the appointment to be deleted is the first appointment
    if (*head == appointmentToDelete) {
        *head = appointmentToDelete->next;
        free(appointmentToDelete);
        printf("Appointment with ID %d deleted successfully.\n", appointment_id);
        return;
    }

    // Find the previous appointment
    struct Appointment* prev = *head;
    while (prev->next != NULL && prev->next != appointmentToDelete) {
        prev = prev->next;
    }

    // Remove the appointment from the linked list
    prev->next = appointmentToDelete->next;
    free(appointmentToDelete);
    printf("Appointment with ID %d deleted successfully.\n", appointment_id);
}
// Delete appointment by patient ID
void deleteAppointmentByPatientID(struct Appointment** head, int patient_id) {
    if (*head == NULL) {
        printf("Appointment list is empty.\n");
        return;
    }

    struct Appointment* current = *head;
    struct Appointment* prev = NULL;

    // Check if the first appointment has the matching Patient_ID
    if (current != NULL && current->Patient_ID == patient_id) {
        *head = current->next;
        free(current);
        printf("Appointment(s) with Patient ID %d deleted successfully.\n", patient_id);
        return;
    }

    // Search for the appointment to be deleted
    while (current != NULL) {
        if (current->Patient_ID == patient_id) {
            prev->next = current->next;
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }

    printf("Appointment(s) with Patient ID %d deleted successfully.\n", patient_id);
}
void cancelAppointment(struct Appointment** appointments){
    int choice;
    int appointmentID;
    int patientID;

    do{
    printf("1- By appointment ID\n");
    printf("2- By patient ID\n");
    printf("3- Back\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
        printf("Enter the appointment ID: ");
        scanf("%d", &appointmentID);
        deleteAppointmentByID(appointments, appointmentID);
        break;

        case 2:
        printf("Enter the patient ID: ");
        scanf("%d", &patientID);
        deleteAppointmentByPatientID(appointments, patientID);
        break;
        
        case 3:
        printf("Going back to the main menu.\n");
        return;

        default:
        printf("Invalid choice. Please try again.\n");
        break;
    }}
    while (choice!=3);

}

//Viewing
// Thread function to print a single node
void* printNode(void* data) {
    struct PrintData* pdata = (struct PrintData*)data;
    struct Appointment* current = pdata->node;

    printf("%d\t\t%d\t\t%d\t\t%d\t\t%d/%d/%d\t%02d:%02d\t\t%s\n",
           current->Appointment_ID,
           current->Patient_ID,
           current->Dentist_ID, //change to name later 
           current->Treatment_ID,
           current->appointment_date.day,
           current->appointment_date.month,
           current->appointment_date.year,
           current->appointment_time.hours,
           current->appointment_time.minutes,
           current->status);

    pthread_exit(NULL);
}
void printAppointments(struct Appointment* head) {
    printf("Viewing all appointments...\n");
    printf("Appointment ID\tPatient ID\tDentist ID\tTreatment ID\tDate\t\tTime\t\tStatus\n");

    struct Appointment* current = head;
    pthread_t threads[100]; 
    int numThreads = 0;

    while (current != NULL) {
        // Create a thread for each node
        struct PrintData* pdata = malloc(sizeof(struct PrintData));
        pdata->node = current;
        pthread_create(&threads[numThreads], NULL, printNode, (void*)pdata);

        current = current->next;
        numThreads++;

        // Limit the number of threads to prevent resource exhaustion
        if (numThreads >= 100) {
            break;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Thread function to print a single dentist
void* printDentist(void* data) {
    struct PrintDentist* pdata = (struct PrintDentist*)data;
    struct Dentist* current = pdata->dentist;

    printf("%-12d%-12s%-18s%-25s%-15s%s\n",
           current->Dentist_ID,
           current->First_Name,
           current->Last_Name,
           current->Email,
           current->Speciality,
           current->Phone_Number);

    pthread_exit(NULL);
}
void printDentists(struct Dentist* head) {
    printf("Viewing all Dentists...\n");
    printf("%-12s%-12s%-18s%-25s%-15s%s\n",
           "Dentist ID", "First Name", "Last Name", "Email", "Speciality", "Phone Number");

    struct Dentist* current = head;
    pthread_t threads[100]; // Adjust the array size as per your requirements
    int numThreads = 0;

    while (current != NULL) {
        // Create a thread for each dentist
        struct PrintDentist* pdata = malloc(sizeof(struct PrintDentist));
        pdata->dentist = current;
        pthread_create(&threads[numThreads], NULL, printDentist, (void*)pdata);

        current = current->next;
        numThreads++;

        // Limit the number of threads to prevent resource exhaustion
        if (numThreads >= 100) {
            break;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Thread function to print a single treatment
void* printTreatment(void* data) {
    struct PrintTreatment* pdata = (struct PrintTreatment*)data;
    struct Treatment* current = pdata->treatment;

    printf("%-15d%-20s%-10d%s\n",
           current->Treatment_ID,
           current->Treatment_name,
           current->price,
           current->Description);

    pthread_exit(NULL);
}
void printTreatments(struct Treatment* head) {
    printf("Viewing all Treatments...\n");
    printf("%-15s%-20s%-10s%s\n",
           "Treatment ID", "Treatment Name", "Price", "Description");

    struct Treatment* current = head;
    pthread_t threads[100]; // Adjust the array size as per your requirements
    int numThreads = 0;

    while (current != NULL) {
        // Create a thread for each treatment
        struct PrintTreatment* pdata = malloc(sizeof(struct PrintTreatment));
        pdata->treatment = current;
        pthread_create(&threads[numThreads], NULL, printTreatment, (void*)pdata);

        current = current->next;
        numThreads++;

        // Limit the number of threads to prevent resource exhaustion
        if (numThreads >= 100) {
            break;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
}

//read from file 
struct Patient* readPatient(struct Patient* h) {
    FILE* fp = fopen("patients", "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open the file 'patients'\n");
        exit(1);
    }

    struct Patient p1;
    struct Patient* current = h;

    while (fread(&p1, sizeof(struct Patient), 1, fp)) {
        struct Patient* newPatient = (struct Patient*)malloc(sizeof(struct Patient));
        newPatient->Patient_ID = p1.Patient_ID;
        strcpy(newPatient->First_Name, p1.First_Name);
        strcpy(newPatient->Last_Name, p1.Last_Name);
        strcpy(newPatient->Email, p1.Email);
        strcpy(newPatient->Phone_Number, p1.Phone_Number);
        newPatient->next = NULL;

        if (current == NULL) {
            // First patient in the linked list
            h = newPatient;
            current = h;
        } else {
            // Append patient to the end of the linked list
            current->next = newPatient;
            current = current->next;
        }
    }

    fclose(fp);
    return h;
}
struct Appointment* readAppointment(struct Appointment* h) {
    FILE* fp = fopen("appointments", "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open the file 'appointments'\n");
        exit(1);
    }

    struct Appointment a1;
    struct Appointment* current = h;

    while (fread(&a1, sizeof(struct Appointment), 1, fp)) {
        struct Appointment* newAppointment = (struct Appointment*)malloc(sizeof(struct Appointment));
        newAppointment->Appointment_ID = a1.Appointment_ID;
        newAppointment->Patient_ID = a1.Patient_ID;
        newAppointment->Dentist_ID = a1.Dentist_ID;
        newAppointment->Treatment_ID = a1.Treatment_ID;
        newAppointment->appointment_date = a1.appointment_date;
        newAppointment->appointment_time = a1.appointment_time;
        strcpy(newAppointment->status, a1.status);
        newAppointment->next = NULL;

        if (current == NULL) {
            // First patient in the linked list
            h = newAppointment;
            current = h;
        } else {
            // Append patient to the end of the linked list
            current->next = newAppointment;
            current = current->next;
        }
    }

    fclose(fp);
    return h;
}
struct Date* readDate(struct Date* h) {
    FILE* fp = fopen("dates", "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open the file 'dates'\n");
        exit(1);
    }

    struct Date d1;
    struct Date* current = h;

    while (fread(&d1, sizeof(struct Date), 1, fp)) {
        struct Date* newDate = (struct Date*)malloc(sizeof(struct Date));
        newDate->day = d1.day;
        newDate->month = d1.month;
        newDate->year = d1.year;
        newDate->next = NULL;

        if (current == NULL) {
            // First patient in the linked list
            h = newDate;
            current = h;
        } else {
            // Append patient to the end of the linked list
            current->next = newDate;
            current = current->next;
        }
    }
    fclose(fp);
    return h;
}
struct Time* readTime(struct Time* h) {
    FILE* fp = fopen("times", "r");
    if (fp == NULL) {
        fprintf(stderr, "Couldn't open the file 'times'\n");
        exit(1);
    }

    struct Time t1;
    struct Time* current = h;

    while (fread(&t1, sizeof(struct Time), 1, fp)) {
        struct Time* newTime = (struct Time*)malloc(sizeof(struct Time));
        newTime->hours = t1.hours;
        newTime->minutes = t1.minutes;
        newTime->next = NULL;

        if (current == NULL) {
            // First patient in the linked list
            h = newTime;
            current = h;
        } else {
            // Append patient to the end of the linked list
            current->next = newTime;
            current = current->next;
        }
    }
    fclose(fp);
    return h;
}

void writeDentists(struct Dentist* dentists) {
    FILE* file = fopen("dentists", "wb");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open the file 'dentists'\n");
        exit(1);
    }

    struct Dentist* current = dentists;
    while (current != NULL) {
        fwrite(current, sizeof(struct Dentist), 1, file);
        current = current->next;
    }

    fclose(file);
}
struct Dentist* readDentists() {
    struct Dentist* dentists = NULL;
    FILE* file = fopen("dentists", "rb");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open the file 'dentists'\n");
        exit(1);
    }

    struct Dentist temp;
    while (fread(&temp, sizeof(struct Dentist), 1, file)) {
        struct Dentist* newDentist = (struct Dentist*)malloc(sizeof(struct Dentist));
        *newDentist = temp;
        newDentist->next = NULL;

        if (dentists == NULL) {
            dentists = newDentist;
        } else {
            struct Dentist* current = dentists;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newDentist;
        }
    }

    fclose(file);
    return dentists;
}

void writeTreatments(struct Treatment* treatments) {
    FILE* file = fopen("treatments", "wb");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open the file 'treatments'\n");
        exit(1);
    }

    struct Treatment* current = treatments;
    while (current != NULL) {
        fwrite(current, sizeof(struct Treatment), 1, file);
        current = current->next;
    }

    fclose(file);
}
struct Treatment* readTreatments() {
    struct Treatment* treatments = NULL;
    FILE* file = fopen("treatments", "rb");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open the file 'treatments'\n");
        exit(1);
    }

    struct Treatment temp;
    while (fread(&temp, sizeof(struct Treatment), 1, file)) {
        struct Treatment* newTreatment = (struct Treatment*)malloc(sizeof(struct Treatment));
        *newTreatment = temp;
        newTreatment->next = NULL;

        if (treatments == NULL) {
            treatments = newTreatment;
        } else {
            struct Treatment* current = treatments;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newTreatment;
        }
    }

    fclose(file);
    return treatments;
}


int main() {
    struct Patient* patients = NULL; // Head of linked list for patients
    struct Dentist* dentists = NULL; // Head of linked list for dentists
    struct Appointment* appointments = NULL; // Head of linked list for appointments
    struct Treatment* treatments = NULL; // Head of linked list for treatments
    struct Date* dates = NULL; // Head of linked list for dates
    struct Time* times = NULL; // Head of linked list for times
    
    // Create patients
    addPatient(&patients, "John", "Doe", "john@example.com", "0999890987");
    addPatient(&patients, "Jane", "Smith", "jane@example.com", "0999890989");
    patients = readPatient(patients);

   
    // Create treatments
    struct Treatment* treatment1 = (struct Treatment*)malloc(sizeof(struct Treatment));
    treatment1->Treatment_ID = 1;
    strcpy(treatment1->Treatment_name, "Cleaning");
    treatment1->price = 100;
    strcpy(treatment1->Description, "Routine teeth cleaning");
    treatment1->next = NULL;
    treatments = treatment1;
    struct Treatment* treatment2 = (struct Treatment*)malloc(sizeof(struct Treatment));
    treatment2->Treatment_ID = 2;
    strcpy(treatment2->Treatment_name, "Braces");
    treatment2->price = 2000;
    strcpy(treatment2->Description, "Orthodontic braces");
    treatment2->next = NULL;
    treatment1->next = treatment2;
    //Write treatments to the file
    writeTreatments(treatment1);
    //Read treatments from the file
    treatments = readTreatments();
    
    
    // Create dentists
    struct Dentist* dentist1 = (struct Dentist*)malloc(sizeof(struct Dentist));
    dentist1->Dentist_ID = 1;
    strcpy(dentist1->First_Name, "Dr. David");
    strcpy(dentist1->Last_Name, "Johnson");
    strcpy(dentist1->Email, "dr.david@example.com");
    strcpy(dentist1->Speciality, "General Dentistry");
    strcpy(dentist1->Phone_Number, "1111111111");
    dentist1->next = NULL;
    dentists = dentist1;


    struct Dentist* dentist2 = (struct Dentist*)malloc(sizeof(struct Dentist));
    dentist2->Dentist_ID = 2;
    strcpy(dentist2->First_Name, "Dr. Sarah");
    strcpy(dentist2->Last_Name, "Williams");
    strcpy(dentist2->Email, "dr.sarah@example.com");
    strcpy(dentist2->Speciality, "Orthodontics");
    strcpy(dentist2->Phone_Number, "2222222222");
    dentist2->next = NULL;
    dentist1->next = dentist2;
    
    // Write dentists to the file
    writeDentists(dentist1);
    // Read dentists from the file
    dentists = readDentists();
    
   
    
    // Create appointments
    
    struct Time* appointmentTime = newTime(&times, 11, 30);
    struct Date* appointmentDate = newDate(&dates, 10, 6, 2023);
    createAppointment(&appointments, 1, 1, 1, *appointmentDate, *appointmentTime, patients, dentists, treatments);
    
    struct Time* appointmentTime1 = newTime(&times, 11, 30);
    struct Date* appointmentDate1 = newDate(&dates, 10, 6, 2023);
    createAppointment(&appointments, 2, 2, 2, *appointmentDate1, *appointmentTime1, patients, dentists, treatments);
    
    struct Time* appointmentTime2 = newTime(&times, 11, 30);
    struct Date* appointmentDate2 = newDate(&dates, 10, 7, 2023);
    createAppointment(&appointments, 5, 4, 2, *appointmentDate2, *appointmentTime2, patients, dentists, treatments);
    dates= readDate(dates);
    times= readTime(times);
    appointments=readAppointment(appointments);
    

    
    int choice;
    do {
        printf("Main menu\n");
        printf("1- View today’s schedule\n");
        printf("2- Create a new appointment\n");
        printf("3- Change the details of an existing appointment\n");
        printf("4- Cancel an existing appointment\n");
        printf("5- View appointment details\n");
        printf("6- View doctors details\n");
        printf("7- View the details of available treatment options\n");
        printf("8- Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewSchedule(appointments,patients,dentists,treatments);
                break;
            case 2:
                 createAppointmentMain(patients,dates,times,appointments,dentists,treatments);
                break;
            case 3:
                changeAppointmentDetails(appointments,dentists,treatments);
                break;
            case 4:
                cancelAppointment(&appointments);
                break;
            case 5:
                printAppointments(appointments);
                break;
            case 6:
                printDentists(dentists);
                break;
            case 7:
                printTreatments(treatments);
                break;
            case 8:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        printf("\n");
    } while (choice != 8);
    

    return 0;
}
