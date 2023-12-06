#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum copy_string_status_code {copy_string_ok, copy_string_error};

enum late_mail_status_code {late_mail_ok, late_mail_fail};

enum received_mail_status_code {received_mail_ok, received_mail_fail};

enum print_mail_status_code {print_mail_ok, print_mail_fail};

enum delete_mail_status_code {delete_mail_ok, delete_mail_fail};

enum add_mail_status_code {add_mail_ok, add_mail_fail};

enum sort_mail_status_code {sort_mail_ok, sort_mail_fail};

typedef struct 
{
    char* data;
    int length;
} String;

typedef struct {
    String city;
    String street;
    unsigned number_house;
    String building;
    unsigned flat_number;
    char id_receiver[6];
} Address;

typedef struct {
    Address receiver;
    double weight; // > 0
    char id[14];
    String create_date; // dd:MM:yyyy hh:mm:ss
    String handle_date; // dd:MM:yyyy hh:mm:ss
    int is_recieved;
} Mail;

typedef struct {
    Address* post_department;
    Mail* mails; // 
    unsigned number_mails;
} Post;

String create_string(const char* data){
    String new_strig;
    new_strig.length = strlen(data);
    new_strig.data = malloc((new_strig.length +1) * sizeof(char));

    if (new_strig.data != NULL){
        strcpy(new_strig.data, data);
    } else {
        new_strig.length = 0;
    }
    return new_strig;
}

void delete_string(String* string){
    if (string->data == NULL){
        return;
    }
    free(string->data);
    string->data = NULL;
    string->length = 0;
    free(string);
}

int strcsl(const void* a, const void* b){
    const String* string_1 = (const String*)a;
    const String* string_2 = (const String*)b;


    const char* data_1 = string_1->data;
    const char* data_2 = string_2->data;

    while (*data_1 != '\0' && *data_2 != '\0') {
        if (*data_1 < *data_2) {
            return -1;
        } else if (*data_1 > *data_2) {
            return 1;
        }

        // Move to the next character
        ++data_1;
        ++data_2;
    }

    // Strings are equal up to the length of the shorter string
    if (*data_1 == '\0' && *data_2 == '\0') {
        return 0;
    } else if (*data_1 == '\0') {
        return -1;
    } else {
        return 1;
    }
}

int compare_strings(const void* a, const void* b){
    const String* string_1 = (const String*)a;
    const String* string_2 = (const String*)b;

    int length_diff = string_1->length - string_2->length;
    if (length_diff != 0){
        return length_diff;
    }

    return strcsl(string_1->data, string_2->data);
}

// Function to copy a string
enum copy_string_status_code copy_string(const String* source, String* copy) {

    // Allocate memory for the copy
    copy->data = malloc((source->length + 1) * sizeof(char));

    if (copy->data == NULL) {
        // Handle memory allocation failure
        copy->length = 0;
        return copy_string_error;
    }

    // Copy the data character by character
    for (int i = 0; i < source->length; i++) {
        copy->data[i] = source->data[i];
    }

    // Add the null terminator
    copy->data[source->length] = '\0';

    // Set the length
    copy->length = source->length;

    return copy_string_ok;
}

// Function to copy_to_string exist dynamic
String* copy_string_exist(String* container, const String* string_copy) {
    
    if (container == NULL || string_copy == NULL) {
        return NULL;
    }

    if (container->length < string_copy->length + 1){

    container->data = realloc(container->data, (string_copy->length + 1) * sizeof(char));
    
        if (container->data == NULL) {
            container->length = 0;
            return container;
        }
    
    container->length = string_copy->length + 1;
    }

    // Copy the data character by character
    for (int i = 0; i < string_copy->length; i++) {
        container->data[i] = string_copy->data[i];
    }

    container->data[string_copy->length] = '\0';


    return container;
}

// Function to concatenate two strings
String* concatenate_string(String* string_1, const String* string_2) {
    if (string_1 == NULL || string_2 == NULL) {
        return NULL;
    }

    int new_length = string_1->length + string_2->length;

    // Reallocate memory for the concatenated string
    string_1->data = realloc(string_1->data, (new_length + 1) * sizeof(char));

    if (string_1->data == NULL) {
        // Handle memory allocation failure
        string_1->length = 0;
        return string_1;
    }

    // Copy the second string into the allocated space
    for (int i = 0; i < string_2->length; i++) {
        string_1->data[string_1->length + i] = string_2->data[i];
    }

    // Add the null terminator
    string_1->data[new_length] = '\0';

    // Update the length
    string_1->length = new_length;

    return string_1;
}

int compare_id_receiver(const void* a, const void* b){
    return ((Mail*)a)->receiver.id_receiver - ((Mail*)b)->receiver.id_receiver;
}

int compare_id_mail(const void* a, const void* b){
    return ((Mail*)a)->id - ((Mail*)b)->id;
}

int compare_create_date(const void* a, const void* b){
    return timegm(((Mail*)a)->create_date, ((Mail*)b)->create_date);
}

int find_mail(const Post* post, const Mail mail){
    int low = 0; int high = post->number_mails - 1, mid, cmp, id_cmp;

    if(post->number_mails < 0){
        return -1;
    }

    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcmp(post->mails[mid].receiver.id_receiver, mail.receiver.id_receiver);

        if (cmp == 0) {
            id_cmp = strcmp(post->mails[mid].id, mail.id);
            if (id_cmp == 0){
                return mid;
            } else if (id_cmp < 0){
                mid++;
            } else {
                mid--;
            }
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

void free_address(Address* address){
    delete_string(&address->building);
    delete_string(&address->street);
    delete_string(&address->city);
    free(address->id_receiver);
    address->flat_number = 0;
    address->number_house = 0;
    free(address);
}

void free_mail(Mail* mail){
    mail->weight = 0;
    free_address(&mail->receiver);
    free(mail->id);
    delete_string(&mail->create_date);
    delete_string(&mail->handle_date);
    free(mail);
}

enum sort_mail_status_code sort_mail(Post* post){
    
    if (post == NULL){
        return sort_mail_fail;
    }

    qsort(post->mails, post->number_mails, sizeof(Mail), compare_id_receiver);

    for (int i = 1; i < post->number_mails; i++) {
        if ((post->mails)[i - 1].receiver.id_receiver == (post->mails)[i].receiver.id_receiver) {
            // Use id comparison
            int result = compare_id_mail(&(post->mails)[i - 1], &(post->mails)[i]);
            
            if (result == 0){
                delete_mail(post, (post->mails)[i]);
            }

            if (result > 0) {
                Mail temp = (post->mails)[i - 1];
                (post->mails)[i - 1] = (post->mails)[i];
                (post->mails)[i] = temp;
            }
        }
    }

    return sort_mail_ok;
}

enum add_mail_status_code add_mail(Post* post, const Mail mail){
    Mail* tmp = (Mail*)realloc(post->mails, sizeof(Mail) * (post->number_mails + 1));
    
    if (tmp == NULL){
        return add_mail_fail;
    }

    post->mails = tmp;
    post->mails[post->number_mails] = mail;
    post->number_mails++; 
    if (sort_mail(post) == sort_mail_fail){
        return add_mail_fail;
    }
    return add_mail_ok;
}

enum delete_mail_status_code delete_mail(Post* post, const Mail mail){
    
    if (post == NULL){
        return delete_mail_fail;
    }

    int index_mail = find_mail(post, mail);

    if (index_mail == -1){
        return delete_mail_fail;
    }

    free_mail(&(post->mails[index_mail]));
    post->number_mails--;

    Mail* tmp = (Mail*)realloc(post->mails, sizeof(Mail) * post->number_mails);

    if (tmp = NULL){
        return delete_mail_fail;
    }

    post->mails = tmp;

    if (sort_mail(post) == sort_mail_fail){
        return delete_mail_fail;
    }

    return delete_mail_ok;
}

enum print_mail_status_code print_mail(const Post* post, char id[14]){

    if(post->number_mails < 0){
        return print_mail_fail;
    }

    for(int i = 0; i < post->number_mails; i++){
        if (strcmp(post->mails[i].id, id) == 0){
            Mail mail = post->mails[i];
            printf("Id: %s, Create_date: %s, Handle_date: %s, Weight: %lf, City: %s, Street: %s, House: %u, Corpus: %s, Flat: %lf\n", 
                    mail.id, mail.create_date, mail.handle_date, mail.weight, mail.receiver.city, mail.receiver.street, mail.receiver.number_house, mail.receiver.building, mail.receiver.flat_number);
            return print_mail_ok;
        }
    }

    return print_mail_fail;
}

time_t parse_date(char *str)
{
    int year, mon, day;
    if (sscanf(str, "%d-%d-%d", &year, &mon, &day) == 3);
    {
        struct tm time;
        memset(&time, 0, sizeof(time)); // Обнуляем структуру
 
        if (mon < 0 || mon > 11 || day < 1 || day > 31 || year < 1900)
            return -1;
 
        time.tm_mon = mon - 1;
        time.tm_mday = day;
        time.tm_year = year - 1900;
 
        time.tm_isdst = -1;
 
        return mktime(&time);
    }
}

enum received_mail_status_code received_mail(const Post* post){
    unsigned number_mails = 0;
    Mail* mails;
    for (int i = 0; i < post->number_mails; i++){
        if (post->mails[i].is_recieved == 1){
            mails = (Mail*)realloc(mails, sizeof(Mail) * (number_mails + 1));

            if (mails == NULL){
                return received_mail_fail;
            }

            mails[number_mails] = post->mails[i];
            number_mails++;
        }
    }
    qsort(mails, number_mails, sizeof(Mail), compare_create_date);

    for (int i = 0; i < number_mails; i++){
        Mail mail = mails[i];
        printf("Id: %s, Create_date: %s, Handle_date: %s, Weight: %lf, City: %s, Street: %s, House: %u, Corpus: %s, Flat: %lf\n", 
                    mail.id, mail.create_date, mail.handle_date, mail.weight, mail.receiver.city, mail.receiver.street, mail.receiver.number_house, mail.receiver.building, mail.receiver.flat_number);
    }
    return received_mail_ok;
}

enum late_mail_status_code late_mail(const Post* post) {
    unsigned number_mails = 0;
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    Mail *mails = NULL;  // Initialize the array
    for (int i = 0; i < post->number_mails; i++) {
        if (post->mails[i].is_recieved == 0) {
            time_t handle_time = parse_date(post->mails[i].handle_date.data);
            if ((handle_time - rawtime) < 1990) {
                mails = (Mail *)realloc(mails, sizeof(Mail) * (number_mails + 1));

                if (mails == NULL) {
                    return late_mail_fail;
                }

                mails[number_mails] = post->mails[i];
                number_mails++;
            }
        }
    }
    qsort(mails, number_mails, sizeof(Mail), compare_create_date);

    for (int i = 0; i < number_mails; i++) {
        Mail mail = mails[i];
        printf("Id: %s, Create_date: %s, Handle_date: %s, Weight: %lf, City: %s, Street: %s, House: %u, Corpus: %s, Flat: %lf\n",
               mail.id, mail.create_date, mail.handle_date, mail.weight, mail.receiver.city, mail.receiver.street,
               mail.receiver.number_house, mail.receiver.building, mail.receiver.flat_number);
    }

    free(mails);\
    return late_mail_ok;
}

