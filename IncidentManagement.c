#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_RESPONDERS 100
typedef enum { false, true } bool;

typedef enum 
{
 no_error, 
 wrong_incident_type,
 duplicate_incident_id,
 out_of_range_priority,
out_of_memory,
no_incidents_to_handle,
incident_was_not_found,
error_in_responder_reading,
no_responders_at_all,
no_responder_for_incident,
no_responder_found_by_name,
}errors_e;

typedef enum
{
    violence,
    vandalism,
    ticketing_disorders,
    obstacle_on_tracks,
    zombie_attack,
    signaling_fault,
    train_technical_difficulties,
    extreme_weather_conditions,
    platform_overcrowding,
    security_alert,
    count_incident_types  //used for counting the number of incident types
} incident_type_e;

char *incident_types[] = {
    
    "violence",
    "vandalism",
    "ticketing_disorders",
    "obstacle_on_tracks",
    "zombie_attack",
    "signaling_fault",
    "train_technical_difficulties",
    "extreme_weather_conditions",
    "platform_overcrowding",
    "security_alert"
};

typedef enum
{
  administrator,
  operator,
  technician,
  analyst,
  count_roles // used for counting the number of roles
}roles_types_e;

char *role_types[] = {
    "administrator",
    "operator",
    "technician",
     "analyst"
};
typedef struct responder_s 
{
    int id;
    char* name;
    int num_handled_incidents; //keeps the number of incidents the responder has handled (closed). should be 0 at start
    int sum_handled_priorities; // sum of the priorities of the incidents the responder has handled . 
    //will be used later for average priority calculation. should be zero at start
    int incident_types[count_incident_types] ; // array whose length is the number of incident types. all values should be zeroat start. eventually , per each 
    // type it should include the number of incidents of this type the responder has handled.
    roles_types_e role;
}responder_s;

typedef struct incident_s
{
    int id; //unique id
    int priority; // the high the number the more urgent the incident
    incident_type_e type; 
    char* description;   
}incident_s;

typedef struct incident_node
{
  incident_s* incident;
  struct incident_node* next; //pointer to next incident node
}incident_node;


 int relation[count_incident_types][count_roles] = {
    {1, 0 , 0, 1},
    {1, 1, 0, 0},
    {1, 0, 1, 1},
    {1, 1, 1, 0},
    {1, 0, 1, 0},
    {1, 1, 0 , 0},
    {0, 1, 0 , 0},
    {1, 0, 1, 0},
    {1, 1, 1, 1},
    {1, 0, 0, 1}   };

typedef struct responder_statistics_s
{
 int id; //id of the responder
 int role; //role of the repsonder
 int num_handled_incidents; //total number of incidents the responder handled
 double avergae_handled_priority; //the average priority of the incidents the responder handled
 incident_type_e most_frequent_type; // the most frequent type the responder handled
}responder_statistics_s;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

incident_s* create_incident(int id, incident_type_e type, int priority,  char* description)
{
  incident_s * incident = (incident_s*)malloc(sizeof(incident_s));
  if(incident == NULL){
    return NULL;
  }
  //id_incident_duplicate if id is allready in 
  incident->id = id;
  incident->type = type;
  incident->priority = priority;
  incident->description = (char*)malloc(sizeof(char) * (strlen(description) + 1));
  if(incident->description == NULL){
    free(incident);
    return NULL;
  }
  strcpy(incident->description,description);

  return incident;
}


void destroy_incident(incident_s* incident)
{
   //need to implement
}



//the function goes over the list and prints incidents id , type, and description
void print_incidents_details(incident_node* head)
{
    //need to implement, use the following line for print
    //incident_node *curr;
    //printf(" incident %d %s %s\n", curr->incident->id, incident_types[curr->incident->type], curr->incident->description);
}


/* function: add_incident
 p_head - pointer to the head of the list 
 the function  adds a new node to the list with the incident with the following properties
 id - the id of the incident 
 type - the type of the incident 
 priority - the priority of the incident 
 description - the description of the incident
  */
errors_e add_incident(incident_node **p_head, int id, incident_type_e type, int priority, char* description)
{ 
    incident_node * curr , *prev = NULL;
    incident_node * node = (incident_node *)malloc(sizeof(incident_node));
    if(node == NULL)
    {
      return out_of_memory;
    }
    node->next = NULL;
    node->incident = create_incident(id,type,priority,description);
    if(node->incident == NULL)
    {
      free(node);
      return out_of_memory;
    }

    if (*p_head == NULL)
    {
      *p_head = node;
    }

    curr = *p_head;
    while (curr != NULL)
    {
      if (curr->incident->id == id) 
        {
        free(node->incident->description);
        free(node->incident);
        free(node);
        return duplicate_incident_id;
        }
      prev = curr;
      curr = curr->next;
    }
    prev->next = node;

return no_error;
}

/* function remove_first_incident 
p_head pointer to the head of the list (pointer to pointer)
p_incident pointer to pointer to incident, here the function will store the removed incident
the memory of the node should be released */
errors_e remove_first_incident(incident_node **p_head, incident_s** p_incident)
{
  //need to impleement
  return no_error;
}


void free_responders(responder_s** responders, int num_responders)
{
  //printf("temp[j].id = %d\ntemp[j].name = %s\ntemp[j].role = %d\n",responders[0]->id,responders[0]->name,responders[0]->role);
   for (int j = 0; j < num_responders; j++) {
        if (responders[j] != NULL) {
            if (responders[j]->name != NULL) {
                free(responders[j]->name);
                responders[j]->name = NULL;  
            }
            free(responders[j]);
            responders[j] = NULL;
        }
    }
  //for testing 
  for(int j = 0 ; j < num_responders; j++)
  printf("temp[j].id = %d\ntemp[j].name = %s\ntemp[j].role = %d\n",responders[j]->id,responders[j]->name,responders[j]->role);

}

/* function load_responders
responders - here responders will be stored
p_num_responders- a pointer to integer where the actual number of valid responders is stored*/
errors_e load_responders(responder_s **responders, int* p_num_responders){
  const char s[2] = " ";
  char *token;
  int id;
  char *name;
  roles_types_e role;

  FILE * file_input = fopen("responders.txt", "r");
  if(file_input == NULL)
  {
    return error_in_responder_reading;
  }
  char buffer[255];
  int j = 0;
  
    while(fgets(buffer,255,file_input)!= NULL){
    if (buffer[0] == '\n'||buffer[0] == '\r') {
            continue;
        }

      token = strtok(buffer, s);
      int i = 0;

      while(token != NULL ) {
      if(i == 0){
        id = atoi(token+3);
        // printf("%d : id\n",id);
      }
      else if(i == 1){
        strcpy(name,token+5);
        // printf("%s : name\n",name);
      }
      else if(i == 2){
        role = atoi(token+5);
        // printf("%d : role\n",role);
      }
        
        token = strtok(NULL, s);
        i++;
      }
      
      if(role < count_roles  && role >= 0){
        responders[j] = (responder_s*)malloc(sizeof(responder_s) * MAX_NUM_RESPONDERS);
        if(responders[j] == NULL){
          fclose(file_input); 
          out_of_memory;
        }
        responders[j]->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
        if(responders[j]->name == NULL)
          continue;
        responders[j]->id = id;
        strcpy(responders[j]->name,name);
        responders[j]->role = role;
        //printf("temp[j].id = %d\ntemp[j].name = %s\ntemp[j].role = %d\n",responders[j]->id,responders[j]->name,responders[j]->role);
        j++;
        (*p_num_responders)++; /*count++*/
      }
  }
  fclose(file_input);
  return no_error;
}


errors_e assign_responder_to_incident(incident_type_e type, responder_s** responders, int num_responders, int* ind_chosen_responder)
{
  //need to implement
  return no_error;
}

//implemented for you, no need to implement
void responder_handles_incident(incident_s *incident, responder_s* responder)
{
   if (responder != NULL)
    {
       responder->num_handled_incidents ++;
       responder->sum_handled_priorities += incident->priority;
       responder->incident_types[incident->type]++;
    }
}

/*function remove_incident_by-id
p_head pointer to the head of the list (pointer to pointer)
id - the id of the incident whose node should be removed
p_incident pointer to pointer to incident, here the function will store the removed incident
the memory of the node should be released inside this function */
errors_e remove_incident_by_id(incident_node **p_head, int id, incident_s** p_incident)
{
   //need to implement
   return no_error;
}
errors_e handle_specific_incident(incident_node **p_head, responder_s ** responders, int num_responders, int id )
{
    incident_s* incident = NULL;
    int chosen_responder_ind;
    errors_e error_code = remove_incident_by_id(p_head, id, &incident);
    if (error_code != no_error)
    {
        return error_code;
    }
 
    error_code = assign_responder_to_incident(incident->type, responders, num_responders,  &chosen_responder_ind);
     if (error_code != no_error)
    {
         destroy_incident(incident);
        return error_code;
    }
    
    printf("responder for incident %d is %s\n",incident->id, responders[chosen_responder_ind]->name);
    responder_handles_incident(incident, responders[chosen_responder_ind]);

     destroy_incident(incident);
     return no_error;
}

errors_e handle_next_incident(incident_node **p_head, responder_s ** responders, int num_responders )
{
    incident_s* incident = NULL;
    int chosen_responder_ind;
    errors_e error_code = remove_first_incident(p_head, &incident);
    if (error_code != no_error)
    {
        return error_code;
    }
 
    error_code = assign_responder_to_incident(incident->type, responders, num_responders,  &chosen_responder_ind);
    
     if (error_code != no_error)
    {
         destroy_incident(incident);
        return error_code;
    }

    responder_s* responder =  responders[chosen_responder_ind];
    printf("responder for incident %d of type %d is %s of role %s\n",incident->id, incident->type, responder->name, role_types[responder->role]);
    responder_handles_incident(incident, responder);

     destroy_incident(incident);
     return no_error;
}


void report_busiest_responder(responder_s** responders, int num_responders, responder_statistics_s* stat)
{
   //need to implement
}



errors_e report_responder_by_name(responder_s** responders, int num_responders, char* name, responder_statistics_s* stat)
{
   //need to implement
   return no_error;
}

void free_incidents_list(incident_node* p_head)
{
   //need to implement
   
}

void print_statistics_report(responder_statistics_s* stat)
{
    printf("printing statistics of user with id %d and role %s\n", stat->id, role_types[stat->role]);
    printf("num of handled indices %d\n", stat->num_handled_incidents);
    printf("average handled priority %f\n", stat->avergae_handled_priority);
    printf("most frequent handled incident type %s\n", incident_types[stat->most_frequent_type]);
}


int main()
{
    incident_node* head= NULL;
    errors_e error_code;
    responder_s* responders[MAX_NUM_RESPONDERS] = {NULL};
    int num_responders = 0;
    error_code =  load_responders(responders, &num_responders);
    if (error_code == no_responders_at_all)
    {
        printf("no responders in the system");
        exit(1);
    }
    printf("number of total responders is %d\n", num_responders);

    
    error_code = add_incident(&head, 303, zombie_attack, 5, "the walkig dead");
    if (error_code!= no_error)
    {
       printf("error code is %d\n", error_code);
    }
    
    
    error_code = add_incident(&head, 303, vandalism, 6, "graffiti");
    if (error_code!= no_error)
    {
      printf("error code is %d\n", error_code);
    }
   
    error_code = add_incident(&head, 101, violence, 9, "massive attack");
     if (error_code!= no_error)
    {
      printf("error code is %d\n", error_code);
    }

    error_code = add_incident(&head, 444, extreme_weather_conditions, 10, "hurricane");
     if (error_code!= no_error)
    {
      printf("error code is %d\n", error_code);
    }

     error_code = add_incident(&head, 555, extreme_weather_conditions, 9, "storm");
     if (error_code!= no_error)
    {
      printf("error code is %d\n", error_code);
    }
    //printf("temp[j].id = %d\ntemp[j].name = %s\ntemp[j].role = %d\n",responders[1]->id,responders[1]->name,responders[1]->role);
    
   error_code = handle_next_incident(&head, responders, num_responders);
    if (error_code!= no_error)
    {
      printf("error code is %d\n", error_code);
    }

    error_code = handle_specific_incident(&head, responders, num_responders, 303);
    if (error_code!= no_error)
    {
      printf("error code is %d\n", error_code);
    }


   print_incidents_details(head);
   error_code = add_incident(&head,666, platform_overcrowding, 1,"rush hours");
   if (error_code!= no_error)
    {
      printf("error code is %d\n", error_code);
    }
    print_incidents_details(head);
    printf("*************\n");
   error_code = handle_next_incident(&head, responders, num_responders);
   error_code = handle_next_incident(&head, responders, num_responders);
    print_incidents_details(head);
    printf("*************\n");
   responder_statistics_s stat;
   error_code = report_responder_by_name(responders, num_responders, "Angelina", &stat);
   if (error_code == no_error)
   {
    print_statistics_report(&stat);
   }
   report_busiest_responder(responders, num_responders, &stat);
   print_statistics_report(&stat);
   
   printf("test\n");
   free_responders(responders, num_responders);
   free_incidents_list(head);
   head = NULL;
   
  
    return 0 ;
}