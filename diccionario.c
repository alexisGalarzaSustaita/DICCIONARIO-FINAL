/*
Objetivo: Crear un diccionario de datos
Autor: G.S.F.A.
Fecha: 02/12/2023
Nota(s): Con entidades, atributos y bloques de datos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef char String[100];

typedef struct
{
  String nombre;
  long prt_sigent;
  long ptr_atrib;
  long direccion;
}TEntidad;

typedef struct
{
  String nombre;
  int tipo; //1.Int 2.Float 3.Char 4.String
  int tam;
  int clave; //0.No Clave 1.Primaria 2.Externa
  long direccion;
  long ptr_sig_a;
  long ptr_ent;//Esta es cuando es una clave externa
}TAtributo;

typedef struct nodA
{
  TAtributo info;
  struct nodA *sig;
}TNodoA;//nodo de los atributos, la sub lista

typedef struct nodE
{
  TEntidad info;
  struct nodE *sig_ent;
  struct nodA *cab_atr;
}TNodoE;//la lista de las entidades, la lista main

typedef struct
{
  void *bloque;
  int tam;
}TBloque;


int menuDic1();
int menuDic2();
int menuDic3();
void crea_diccionario(char *nom_arch);
void manipula_dic(char *nom_dic);

//ENTIDADES:
void captura_entidad(TEntidad *e);
void imprime_entidad(TEntidad e);
void inserta_entidad(TEntidad e,char nom_arch[]);
void elimina_entidad(char nom_arch[], char nombre_entidad[]);
void modifica_entidad(char nom_ach[], char nom_entidad[], char nuevo_nombre[]);
void consulta_entidad(char nom_arch[], char nombre_entidad[]);
TNodoE *busca_entidad(char *nom_ent, TNodoE *cab_dic);

//ATRIBUTOS:
void captura_atributo(TAtributo *a);
void inserta_atributo(char *nom_arch, char *nom_ent,TAtributo a);
void elimina_atr(char nom_dic[], char nom_entidad[], char nom_atr[]);
void modifica_atr(char nom_ach[], char nom_entidad[], char nom_atributo[],char nuevo_nombre[], int nuevo_tipo);
void consulta_atr(char nom_dic[], char nom_entidad[], char nom_atributo[]);
void imprime_atributo(TAtributo a);

//LISTAS:
TNodoE *crea_nodE(TEntidad entidad);
TNodoA *crea_nodA(TAtributo atributo);
void imprime_DicLis(TNodoE *c);
TNodoE *inserta_finalE(TNodoE **cab,TEntidad d);
void inserta_finalA(TNodoA **cab, TAtributo d);
TNodoE *Carga_Diccionario(char nom_dic[]);

//DATOS Y BLOQUES 
void manipula_datos(char *nom_dic);
TBloque crea_bloque(TNodoE *e);
void captura_bloque(TNodoE *e,TBloque b);
void imprime_bloque(TNodoE *e,TBloque b);
int compara_bloque(TBloque b1, TBloque b2, TNodoE *e);
void inserta_Bloque(TNodoE *e, TBloque b);
void imprime_Tabla(TNodoE *e);
void elimina_bloque(TNodoE *ent, TBloque b);
TBloque busca_Bloque(TNodoE *e);

void imprime_diccionario(char *nom_dic);

int main()
{
  int op;
  char nom_dic[50];
  do{
    op=menuDic1();
    switch(op)
      {
        case 1:printf("Nombre del Diccionario:");
              scanf("%s",nom_dic);

              // Verificar si el archivo ya existe
              if (access(nom_dic, F_OK) != -1) 
              {
                // El archivo existe
                printf("El diccionario ya existe. Intente con otro nombre.\n");
              } 
              else 
              {
                crea_diccionario(nom_dic);
                manipula_dic(nom_dic);
              }
              break;
        case 2: printf("Nombre del Diccionario:");
                scanf("%s",nom_dic);

                // Verificar si el archivo existe
                if (access(nom_dic, F_OK) != -1) 
                {
                  // El archivo existe, entonces se puede manipular
                  manipula_dic(nom_dic);
                } 
                else 
                {
                  // El archivo no existe
                  printf("El diccionario no existe. Por favor, cree el diccionario primero.\n");
                }
                break;
      }
  }while(op!=3);
}

void crea_diccionario(char *nom_arch)
{
  FILE *pf;
  long cab=-1;

  pf = fopen(nom_arch, "wb");
  if(pf != NULL)
  {
    fwrite(&cab,sizeof(long),1,pf);
    fclose(pf);
  }
}

int menuDic1()
{
  int op;

  printf("1.Crea diccionario\n");
  printf("2.Abrir diccionario\n");
  printf("3.Salir\n");
  printf("Opcion: ");
  scanf("%d", &op);
  return op;
}

int menuDic2()
{
  int op;

  printf("\n1.Alta de Entidad\n");
  printf("2.Baja de Entidad\n");
  printf("3.Modificación de Entidad\n");
  printf("4.Consulta de Entidad\n");
  printf("5.Alta de Atributo\n");
  printf("6.Baja de Atributo\n");
  printf("7.Modificación de Atributo\n");
  printf("8.Consulta de Atributo\n");
  printf("9 Imprime Diccionario\n");
  printf("10.Datos\n");
  printf("11.Salir\n");
  printf("Opcion: ");
  scanf("%d", &op);
  printf("\n");
  return op;
}

void manipula_dic(char *nom_dic)
{
  int op;
  TEntidad ent;
  TAtributo atributo;
  char nom_ent[100], nuevoNombre[100], nom_atributo[100];
  int nuevoTipo;

  do
  {
    op=menuDic2();
    switch(op)
    {
      case 1:
        captura_entidad(&ent);
        imprime_entidad(ent);
        inserta_entidad(ent, nom_dic);
        break;
      case 2:
        printf("Nombre de la entidad a eliminar:");
        scanf("%s",nom_ent);
        elimina_entidad(nom_dic, nom_ent);
        break;
      case 3:
        printf("Nombre de la entidad a modificar: ");
        scanf("%s", nom_ent);
        printf("Nuevo nombre de la entidad: ");
        scanf("%s", nuevoNombre);
        modifica_entidad(nom_dic, nom_ent, nuevoNombre);
        break;
      case 4:
        printf("Nombre de la entidad a consultar: ");
        scanf("%s", nom_ent);
        consulta_entidad(nom_dic, nom_ent);
        break;
      case 5:
        captura_atributo(&atributo);
        printf("Nombre de la entidad a la que se agregará el atributo: ");
        scanf("%s", nom_ent);
        inserta_atributo(nom_dic, nom_ent, atributo);
        break;
      case 6:
        printf("Nombre de la entidad a la que se eliminará el atributo: ");
        scanf("%s", nom_ent);
        printf("Nombre del atributo a eliminar: ");
        scanf("%s", nom_atributo);
        elimina_atr(nom_dic, nom_ent, nom_atributo);
        break;
      case 7:
        printf("Nombre de la entidad a la que se modificará el atributo: ");
        scanf("%s", nom_ent);
        printf("Nombre del atributo a modificar: ");
        scanf("%s", nom_atributo);
        printf("Nuevo nombre del atributo: ");
        scanf("%s", nuevoNombre);
        printf("Nuevo tipo del atributo: ");
        printf("\n1. Entero\n");
        printf("2. Flotante\n");
        printf("3. Caracter\n");
        printf("4. Cadena\n");
        scanf("%d", &nuevoTipo);
        modifica_atr(nom_dic, nom_ent, nom_atributo, nuevoNombre, nuevoTipo);
        break;
      case 8:
        printf("Nombre de la entidad a la que se consultará el atributo: ");
        scanf("%s", nom_ent);
        printf("Nombre del atributo a consultar: ");
        scanf("%s", nom_atributo);
        consulta_atr(nom_dic, nom_ent, nom_atributo);
        break;
      case 9:
        imprime_diccionario(nom_dic);
        break;
      case 10:
        manipula_datos(nom_dic);
        break;
    }
  }while(op!=11);
}

void manipula_datos(char *nom_dic)
{
  int op, clave_p;
  TNodoE *cab_dic, *aux;
  char nom_ent[100];
  TBloque b1,b2;

  cab_dic=Carga_Diccionario(nom_dic);
  do
  {
    op=menuDic3();
    switch(op)
    {
      case 1:printf("\n(ALTA) Entidad:");
            scanf("%s",nom_ent);
             aux=busca_entidad(nom_ent, cab_dic);
             b1=crea_bloque(aux);
             captura_bloque(aux,b1);
             printf("\n");
             imprime_bloque(aux,b1);
             printf("\n");
             inserta_Bloque(aux, b1);
             break;
      case 2:
            printf("\n(BAJA) Entidad: ");
            scanf("%s", nom_ent);
            aux=busca_entidad(nom_ent, cab_dic);
            if(aux)
            {
              b1=busca_Bloque(aux);
              if(b1.bloque)
                elimina_bloque(aux, b1);
              else
                printf("No existe el bloque\n");
            }
            else
              printf("la entidad no existe\n");
            break;
      case 3:
            printf("\nNombre de la entidad (bloque) a modificar: ");
            scanf("%s",nom_ent);
            aux=busca_entidad(nom_ent, cab_dic);
            if(aux)
            {
              b2 = busca_Bloque(aux);
              if(b2.bloque)
              {
                b1 = crea_bloque(aux);
                captura_bloque(aux,b1);
                elimina_bloque(aux,b2);
                inserta_Bloque(aux,b1);
              }
              else
                printf("\nNo existe el bloque a modificar\n");
            }
            break;
      case 4:
            printf("\nNombre de la entidad (bloque) a consultar: ");
            scanf("%s",nom_ent);
            aux=busca_entidad(nom_ent, cab_dic);
            if(aux)
            {
              b1=busca_Bloque(aux);
              printf("\n");
              if(b1.bloque)
                imprime_bloque(aux,b1);
              else
                printf("\nNo existe el bloque a consultar \n");
            }
            else
              printf("\nno existe la entidad ingresada \n");            
            break;
      case 5:printf("\nEntidad:");
              scanf("%s",nom_ent);
              printf("\n");
             aux=busca_entidad(nom_ent, cab_dic);
            if(aux)
               imprime_Tabla(aux);
             else
               printf("\nno existe la entidad\n");            
             break;
     }
  }while(op!=6);
}

int menuDic3()
{
  int op;
  printf("\n1.Alta de Bloques\n");
  printf("2.Baja de Bloques\n");
  printf("3.Modificación de Bloques\n");
  printf("4.Consulta de Bloques\n");
  printf("5.Muestra Tabla\n");
  printf("6.Salir\n");
  printf("Opcion: ");
  scanf("%d", &op);
  return op;
}

void captura_entidad(TEntidad *e)
{
  printf("Nombre de la Entidad:");
  scanf("%s",e->nombre);
  e->direccion=e->prt_sigent=e->ptr_atrib=-1L;
}

void imprime_entidad(TEntidad e)
{
  printf("\n...........Entidad..............\n");
  printf("Nombre de la Entidad: %s\n",e.nombre);
  printf("Direccion: %ld\n",e.direccion);
  printf("Direccion Sig Ent: %ld\n",e.prt_sigent);
  printf("Direccion Sig Atr: %ld\n",e.ptr_atrib);
  printf(".................................\n");
}

void inserta_entidad(TEntidad e,char nom_arch[])
{
  long cab, dir;
  TEntidad aux, aux_ant;
  FILE *f;

  f=fopen(nom_arch,"rb+");
  if(f!=NULL)
  {
    fseek(f, 0,SEEK_END);//Mover al final del archivo
    dir=ftell(f);
    e.direccion=dir;
    fseek(f, 0,SEEK_SET);//Mover al inicio del archivo
    fread(&cab,sizeof(long),1,f);//leer la cabecera
    if(cab==-1L)
    {
      cab=dir;
      fseek(f, 0,SEEK_SET);//Mover al inicio del archivo
      fwrite(&cab,sizeof(long),1,f);//Actualizo la cabecera
      fseek(f, dir,SEEK_SET);
      fwrite(&e,sizeof(TEntidad),1,f);
    }
    else
    {
      fseek(f, cab,SEEK_SET);
      fread(&aux,sizeof(TEntidad),1,f);
      if(strcmp(aux.nombre,e.nombre)>0)
      {
        e.prt_sigent=cab;
        cab=e.direccion;
        fseek(f, 0,SEEK_SET);//Mover al inicio del archivo
        fwrite(&cab,sizeof(long),1,f);//Actualizo la cabecera

      }
      else
      {
        while(strcmp(aux.nombre,e.nombre)<0 && aux.prt_sigent!=-1)
          {
            aux_ant=aux;
            fseek(f, aux_ant.prt_sigent, SEEK_SET);
            fread(&aux,sizeof(TEntidad),1,f);
          }
          if(strcmp(aux.nombre,e.nombre)>0)
          {
            aux_ant.prt_sigent=e.direccion;
            e.prt_sigent=aux.direccion;
            fseek(f, aux_ant.direccion,SEEK_SET);
            fwrite(&aux_ant,sizeof(TEntidad),1,f);
          }
          else
          {
            aux.prt_sigent=e.direccion;
            fseek(f, aux.direccion,SEEK_SET);
            fwrite(&aux,sizeof(TEntidad),1,f);
          }
      }
      fseek(f, e.direccion,SEEK_SET);
      fwrite(&e,sizeof(TEntidad),1,f);
    }
    fclose(f);
  }
}

void elimina_entidad(char nom_arch[], char nombre_entidad[])
{
  TEntidad aux, aux_ant;
  FILE *f;
  long cab;

  f=fopen(nom_arch,"rb+");
  if (f)
  {
    fread(&cab,sizeof(long),1,f);

    if(cab!=-1L)
    {
      fseek(f,cab,SEEK_SET);
      fread(&aux,sizeof(TEntidad),1,f);
      if(strcmp(aux.nombre,nombre_entidad)==0)
      {
        cab=aux.prt_sigent;
        fseek(f,0,SEEK_SET);
        fwrite(&cab,sizeof(long),1,f);
      }
    else
      {
        while(strcmp(aux.nombre,nombre_entidad)!=0 && aux.prt_sigent!=-1L)
        {
          aux_ant=aux;
          fseek(f,aux.prt_sigent,SEEK_SET);
          fread(&aux,sizeof(TEntidad),1,f);
        }
        if(strcmp(aux.nombre,nombre_entidad)==0)
        {
          aux_ant.prt_sigent=aux.prt_sigent;
          fseek(f,aux_ant.direccion,SEEK_SET);
          fwrite(&aux_ant,sizeof(TEntidad),1,f);
        }
      }
    }
    fclose(f);
  }
}

void modifica_entidad(char nom_ach[], char nom_entidad[], char nuevo_nombre[])
{
  FILE *f;
  long dir_original, cab;
  TEntidad aux;
  f=fopen(nom_ach,"rb+");
  if (!f)
  {
    printf("No se pudo abrir el diccionario");
    return;
  }
  fread(&cab, sizeof(long),1,f);
  int entidad_encontrada=0;
  while (cab!=-1&& !entidad_encontrada)
  {
    fseek(f,cab,SEEK_SET);
    fread(&aux,sizeof(TEntidad),1,f);
    if (strcmp(aux.nombre, nom_entidad) == 0)
    {
      entidad_encontrada=1;
    } 
    else
    {
      cab=aux.prt_sigent;
    }
  }
  if (entidad_encontrada)
  {
    // Guarda la dirección actual de la entidad
    dir_original = aux.direccion;

    // Elimina la entidad original
    elimina_entidad(nom_ach, nom_entidad);

    // Crea una nueva entidad con el nombre modificado
    strcpy(aux.nombre, nuevo_nombre);
    inserta_entidad(aux, nom_ach);

    printf("Entidad modificada con exito.\n");
  } 
  else
  {
    printf("Entidad no encontrada.\n");
  }
  fclose(f);
}

void consulta_entidad(char nom_arch[], char nombre_entidad[])
{
    TEntidad aux;
    TAtributo aux_a;
    FILE *f;
    long cab, dira;
    f = fopen(nom_arch, "rb");
    if (f)
    {
        fread(&cab, sizeof(long), 1, f);
        while (cab != -1L)
        {
            fseek(f, cab, SEEK_SET);
            fread(&aux, sizeof(TEntidad), 1, f);
            if (strcmp(aux.nombre, nombre_entidad) == 0)
            {
                imprime_entidad(aux);
                dira=aux.ptr_atrib;
                while(dira!=-1L)
                 {
                  fseek(f,dira,SEEK_SET);
                  fread(&aux_a,sizeof(TAtributo), 1, f);
                  imprime_atributo(aux_a);
                   dira=aux_a.ptr_sig_a;
                 }
                fclose(f);
                return;
            }
            cab = aux.prt_sigent;
        }
        printf("Entidad '%s' no encontrada en el diccionario.\n", nombre_entidad);
        fclose(f);
    }
    else
        printf("Error al abrir el archivo del diccionario.\n");
}

void captura_atributo(TAtributo *a)
{
  printf("Nombre del atributo:");
  scanf("%s",a->nombre);
  printf("Tipo (1.Int 2.Float 3.Char 4.String):");
  scanf("%d",&a->tipo);
  switch(a->tipo)
  {
    case 1:a->tam=sizeof(int);
           break;
    case 2:a->tam=sizeof(float);
           break;
    case 3:a->tam=sizeof(char);
           break;
    case 4:printf("Tamanio:");
           scanf("%d",&a->tam);
           break;
  }
  printf("Tipo de clave (0.No Clave 1.Primaria 2.Externa):");
  scanf("%d",&a->clave);
  a->direccion=a->ptr_ent=a->ptr_sig_a=-1L;
}
//Santillan Salazar Ricardo Adrian
void inserta_atributo(char *nom_arch, char *nom_ent,TAtributo a)
{
  FILE *f;
  TEntidad e;
  TAtributo atr;
  long pos;
  int enc=0;

  f=fopen(nom_arch,"rb+");
  if(f)
  {
    fread(&pos,sizeof(long),1,f);
    while(pos!=-1 && !enc)
    {
     fseek(f,pos,SEEK_SET);
     fread(&e,sizeof(TEntidad),1,f);
     if(strcmp(e.nombre,nom_ent)==0)
       enc=1;
     else
       pos=e.prt_sigent;
    }
    if(enc)
    {
      fseek(f,0,SEEK_END);
      a.direccion=ftell(f);
      if(e.ptr_atrib==-1L)
      {
        e.ptr_atrib=a.direccion;
        fseek(f,e.direccion,SEEK_SET);
        fwrite(&e,sizeof(TEntidad), 1, f);
      }
      else
      {
        pos=e.ptr_atrib;
        while(pos!=-1)
          {
            fseek(f,pos,SEEK_SET);
            fread(&atr,sizeof(TAtributo),1,f);
            pos=atr.ptr_sig_a;
          }
        atr.ptr_sig_a=a.direccion;
        fseek(f,atr.direccion, SEEK_SET);
        fwrite(&atr, sizeof(TAtributo),1,f);
      }
      fseek(f,a.direccion,SEEK_SET);
      fwrite(&a, sizeof(TAtributo),1,f);
    }
    fclose(f);
  }
}

void elimina_atr(char nom_dic[], char nom_entidad[], char nom_atr[])
{
  TEntidad e;
  TAtributo aux, ant;
  long cab, pos;
  FILE *f;
  int enc = 0;

  f = fopen(nom_dic, "rb+");
  if (!f)
  {
    printf("no existe el dicc ");
    return;
  }
  fread(&cab, sizeof(long), 1, f);
  while (cab != -1 && !enc)
  {
    fseek(f, cab, SEEK_SET);
    fread(&e, sizeof(TEntidad), 1, f);
    if (strcmp(e.nombre, nom_entidad) == 0)
      enc = 1;
    else
      cab = e.prt_sigent;
  }
  if (enc == 1)
  {
    pos = e.ptr_atrib;
    enc = 0;
    while (pos != -1 && !enc)
    {
      fseek(f, pos, SEEK_SET);
      fread(&aux, sizeof(TAtributo), 1, f);
      if (strcmp(aux.nombre, nom_atr) == 0)

        enc = 1;
      else
      {
        ant = aux;
        pos = aux.ptr_sig_a;
      }
    }
    if (enc)
    {
      if (pos == e.ptr_atrib)
      {
        e.ptr_atrib = aux.ptr_sig_a;
        fseek(f, e.direccion, SEEK_SET);
        fwrite(&e, sizeof(TEntidad), 1, f);
      }
      else
      {
        ant.ptr_sig_a = aux.ptr_sig_a;
        fseek(f, ant.direccion, SEEK_SET);
        fwrite(&ant, sizeof(TAtributo), 1, f);
      }
    }
    else
      printf("atributo no encontrada");
  }
  else
    printf("entidad no encontrada");
  fclose(f);
}

void modifica_atr(char nom_ach[], char nom_entidad[], char nom_atributo[],char nuevo_nombre[], int nuevo_tipo)
{
  FILE *f;
  long cab;
  TEntidad aux_ent;
  TAtributo aux_atr;
  f = fopen(nom_ach, "rb+");
  if (!f)
  {
    printf("No se pudo abrir el diccionario");
    return;
  }
  fread(&cab, sizeof(long), 1, f);
  int entidad_encontrada = 0;
  int atributo_encontrado = 0, nueva_long=0;
  while (cab != -1 && !entidad_encontrada)
  {
    fseek(f, cab, SEEK_SET);
    fread(&aux_ent, sizeof(TEntidad), 1, f);
    if (strcmp(aux_ent.nombre, nom_entidad) == 0)
    {
      entidad_encontrada = 1;
    }
    else
    {
      cab = aux_ent.prt_sigent;
    }
  }
  if (entidad_encontrada)
  {
    long dir_atr = aux_ent.ptr_atrib;
    while (dir_atr != -1 && !atributo_encontrado)
    {
      fseek(f, dir_atr, SEEK_SET);
      fread(&aux_atr, sizeof(TAtributo), 1, f);
      if (strcmp(aux_atr.nombre, nom_atributo) == 0)
      {
        atributo_encontrado = 1;
      }
      else
      {
        dir_atr = aux_atr.ptr_sig_a;
      }
    }
    if (atributo_encontrado)
    {
      strcpy(aux_atr.nombre, nuevo_nombre);
      aux_atr.tipo = nuevo_tipo;
      switch(nuevo_tipo)
      {
        case 1:
          aux_atr.tam = sizeof(int);
          break;
        case 2:
          aux_atr.tam = sizeof(float);
          break;
        case 3:
          aux_atr.tam = sizeof(char);
          break;
        case 4:
          printf("¿Tamaño de la nueva cadena?");
          scanf("%d", &nueva_long);
          aux_atr.tam = nueva_long;
          break;
      }
      fseek(f, aux_atr.direccion, SEEK_SET);
      fwrite(&aux_atr, sizeof(TAtributo), 1, f);
      printf("Atributo modificado con éxito.\n");
    }
    else
    {
      printf("Atributo no encontrado.\n");
    }
  }
  else
  {
    printf("Entidad no encontrada.\n");
  }

  fclose(f);
}


void imprime_atributo(TAtributo a)
{
  printf("\n----------Atributo----------\n");
  printf("Nombre: %s\n",a.nombre);
  printf("Tipo (1.Int 2.Float 3.Char 4.String): %d\n",a.tipo);
  printf("Tamano: %d\n",a.tam);
  printf("Tipo de clave (0.No Clave 1.Primaria 2.Externa): %d\n",a.clave);
  printf("Direccion: %ld\n",a.direccion);
  printf("Direccion entidad ext: %ld\n",a.ptr_ent);
  printf("Direccion del siguiente atributo: %ld\n",a.ptr_sig_a);
   printf("------------------------------\n");
}
//Santillan Salazar Ricardo Adrian
void consulta_atr(char nom_dic[], char nom_entidad[], char nom_atributo[])
{

    FILE *f;
    long cab;
    TEntidad aux_ent;
    TAtributo aux_atr;

    f = fopen(nom_dic, "rb");
    if (!f)
    {
        printf("Error al abrir el archivo del diccionario.\n");
        return;
    }

    fread(&cab, sizeof(long), 1, f);
    int entidad_encontrada = 0;
    while (cab != -1 && !entidad_encontrada)
    {
        fseek(f, cab, SEEK_SET);
        fread(&aux_ent, sizeof(TEntidad), 1, f);
        if (strcmp(aux_ent.nombre, nom_entidad) == 0)
        {
            entidad_encontrada = 1;
        }
        else
        {
            cab = aux_ent.prt_sigent;
        }
    }

    if (entidad_encontrada)
    {
        long dir_atr = aux_ent.ptr_atrib;
        int atributo_encontrado = 0;

        while (dir_atr != -1 && !atributo_encontrado)
        {
            fseek(f, dir_atr, SEEK_SET);
            fread(&aux_atr, sizeof(TAtributo), 1, f);
            if (strcmp(aux_atr.nombre, nom_atributo) == 0)
            {
                atributo_encontrado = 1;
            }
            else
            {
                dir_atr = aux_atr.ptr_sig_a;
            }
        }

        if (atributo_encontrado)
        {
            printf("\n...........Atributo..............\n");
            printf("Atributo encontrado:\n");
            printf("Nombre: %s\n", aux_atr.nombre);
            printf("Tipo: %d\n", aux_atr.tipo);
            printf("Tamaño: %d\n", aux_atr.tam);
            printf("Clave: %d\n", aux_atr.clave);
            printf("Dirección: %ld\n", aux_atr.direccion);
            printf(".................................\n");
        }
        else
        {
            printf("Atributo no encontrado en la entidad '%s'.\n", nom_entidad);
        }
    }
    else
    {
        printf("Entidad '%s' no encontrada en el diccionario.\n", nom_entidad);
    }

    fclose(f);
}

TNodoE *crea_nodE(TEntidad entidad)
{
   TNodoE *aux= (TNodoE*)malloc(sizeof(TNodoE));
   if(aux!=NULL)
   {
      aux->info=entidad;
      aux->cab_atr=NULL;
      aux->sig_ent=NULL;
   }
   return aux;
}

TNodoA *crea_nodA(TAtributo atributo)
{
   TNodoA *aux=(TNodoA*)malloc(sizeof(TNodoA));
   if(aux!=NULL)
   {
      aux->info=atributo;
      aux->sig=NULL;
   }
   return aux;
}

TNodoE* inserta_nodE(TNodoE **cab, TEntidad d)
{
   TNodoE *aux;
   if(*cab==NULL)
   {
      *cab=aux=crea_nodE(d);
   }
   else
   {
      aux=inserta_nodE(&(*cab)->sig_ent, d);
   }
   return aux;
}

void inserta_nodA(TNodoA **cab, TAtributo d)
{
   if(*cab==NULL)
   {
      *cab=crea_nodA(d);
   }
   else
   {
      inserta_nodA(&(*cab)->sig, d);
   }
}

TNodoE *Carga_Diccionario(char *nom_arch)
{
   TEntidad aux;
   TAtributo aux_a;
   FILE *f;
   long dir,dir_a;
   TNodoE *cab=NULL, *nod_ent;

   f=fopen(nom_arch, "rb");
   if(f){
      fread(&dir, sizeof(long), 1, f);
      while(dir!=-1)
      {
         fseek(f, dir, SEEK_SET);
         fread(&aux, sizeof(TEntidad), 1, f);
         nod_ent=inserta_nodE(&cab,aux);
         dir=aux.prt_sigent;
         dir_a=aux.ptr_atrib;
         while (dir_a!=-1L)
         {
            fseek(f,dir_a,SEEK_SET);
            fread(&aux_a,sizeof (TAtributo),1,f);
            inserta_nodA(&nod_ent->cab_atr,aux_a);
            dir_a=aux_a.ptr_sig_a;
         }
      }
      fclose(f);
   }
   return cab;
}

void imprime_DicLis(TNodoE *c)
{
  TNodoA *correA;
  TNodoE *correE;
  correE = c;
  while (correE)
  {
    imprime_entidad(correE->info);
    correA = correE->cab_atr;
    while (correA)
    {
      imprime_atributo(correA->info);
      correA = correA->sig;
    }
    correE = correE->sig_ent;
  }
}

TNodoE *busca_entidad(char *nom_ent, TNodoE *cab_dic)
{
  TNodoE *aux=NULL;
  if(cab_dic)
  {
    if(strcmp(cab_dic->info.nombre,nom_ent)==0)
      aux=cab_dic;
    else
      aux=busca_entidad(nom_ent,cab_dic->sig_ent);
  }
  return aux;
}

TBloque crea_bloque(TNodoE *e)
{
   TBloque b={NULL,0};
   TNodoA *a=e->cab_atr;
   while(a!=NULL)
   {
      b.tam+=a->info.tam;
      a=a->sig;
   }
   b.tam+=sizeof(long)*2;
   b.bloque=malloc(b.tam);
   return b;
}

void captura_bloque(TNodoE *ent, TBloque b)
{
   TNodoA *aux_a;
   int desp=0;
   aux_a=ent->cab_atr;
   printf("%s\n", ent->info.nombre);
   while(aux_a)
   {
      printf("%s: ",aux_a->info.nombre);
      switch (aux_a->info.tipo)
      { //1-int, 2-float, 3-char 4-string
         case 1:
            scanf("%d", (int*)(b.bloque+desp));
            break;
         case 2:
            scanf("%f", (float*)(b.bloque+desp));
            break;         
         case 3:
            scanf("%c", (char*)(b.bloque+desp));
            break;        
         case 4:
            scanf("%s", (char*)(b.bloque+desp));
            break;         
      }
      desp+=aux_a->info.tam;
      aux_a=aux_a->sig;
   }
   *((long*)(b.bloque+desp))=-1L;//su direccion en la que esta
   desp+=sizeof(long);
   *((long*)(b.bloque+desp))=-1L;//direccion de la siguiente
}

void imprime_bloque(TNodoE *ent, TBloque b)
{
   TNodoA *aux_a;
   int desp=0;
   aux_a=ent->cab_atr;
   printf("%s\n", ent->info.nombre);
   while(aux_a){
      printf("%s: ",aux_a->info.nombre);
      switch (aux_a->info.tipo)
      {
         case 1:           
            printf("%d\n", *(int*)(b.bloque+desp));
            break;
         case 2:
            printf("%f\n", *(float*)(b.bloque+desp));
            break;
         case 3:
            printf("%c\n", *(char*)(b.bloque+desp));
            break;
         case 4:
            printf("%s\n", (char*)(b.bloque+desp));
            break;
      }
      desp+=aux_a->info.tam;
      aux_a=aux_a->sig;
   }
   printf("Dir:%ld\n",*((long*)(b.bloque+desp)));
   desp+=sizeof(long);
   printf("Sig Dir:%ld\n",*((long*)(b.bloque+desp)));
}

int compara_bloque(TBloque b1, TBloque b2, TNodoE *e)
{
  TNodoA *a=e->cab_atr;
  int desp=0;
  int b=0;

  while(a && a->info.clave!=1)
    {
      desp+=a->info.tam;
      a=a->sig;
    }
  if(a)
  {
    switch(a->info.tipo)
      {
        case 1:if(*((int*)(b1.bloque+desp))>*((int*)(b2.bloque+desp)))
                b=1;
               else
                if(*((int*)(b1.bloque+desp))<*((int*)(b2.bloque+desp)))
                  b=-1;
              break;
        case 2:if(*((float*)(b1.bloque+desp))>*((float*)(b2.bloque+desp)))
                 b=1;
               else
                  if(*((float*)(b1.bloque+desp))<*((float*)(b2.bloque+desp)))
                     b=-1;
               break;
        case 3:if(*((char*)(b1.bloque+desp))>*((char*)(b2.bloque+desp)))
                 b=1;
               else
                 if(*((char*)(b1.bloque+desp))<*((char*)(b2.bloque+desp)))
                   b=-1;
               break;
        case 4:b=strcmp((char*)(b1.bloque+desp), (char*)(b2.bloque+desp));
               break;
      }
  }
  return(b);
}

void inserta_Bloque(TNodoE *e, TBloque b)
{
   char nom_arch[100];
   FILE *f;
   long cab=-1,pos=-1, pos_ant,pos1=-1;
   TBloque bAct={NULL,0}, bAnt={NULL,0};
   int res=0;

   strcpy(nom_arch, e->info.nombre);
   strcat(nom_arch, ".dat");
   f=fopen(nom_arch, "rb+");

   if(f==NULL)
   {
      f=fopen(nom_arch, "wb+");
      fwrite(&cab, sizeof(long), 1, f);
      cab=ftell(f);
      *((long*)(b.bloque+b.tam-sizeof(long)*2))=cab;
      fseek(f, 0, SEEK_SET);
      fwrite(&cab, sizeof(long), 1, f);
      pos=*((long*)(b.bloque+b.tam-sizeof(long)*2));
      fseek(f, pos, SEEK_SET);
      fwrite(b.bloque, b.tam, 1, f);
   }
   else
   {
      fread(&cab, sizeof(long), 1, f);
      if(cab!=-1L)
      {
         bAct=crea_bloque(e);
         fseek(f, cab, SEEK_SET);
         fread(bAct.bloque, bAct.tam, 1, f);
         //imprime_bloque(e, bAct);
      }
      if(cab==-1L || compara_bloque(bAct, b, e)>0)
      {
         *((long*)(b.bloque+b.tam-sizeof(long)))=cab;
         fseek(f, 0, SEEK_END);
         pos=ftell(f);
         *((long*)(b.bloque+b.tam-sizeof(long)*2))=pos;
         cab=pos;
         fseek(f, 0, SEEK_SET);
         fwrite(&cab, sizeof(long), 1, f);
      }
      else
      {
         while(compara_bloque(bAct, b, e)<0 && *((long*)(bAct.bloque+bAct.tam-sizeof(long)))!=-1L)
         {
            if(bAnt.bloque!=NULL)
            {
               free(bAnt.bloque);
            }
            bAnt=bAct;
            //imprime_bloque(e, bAnt);
            bAct=crea_bloque(e);
            pos_ant=*((long*)(bAnt.bloque+bAnt.tam-sizeof(long)));
            fseek(f, pos_ant, SEEK_SET);
            fread(bAct.bloque, bAct.tam, 1, f);
            //imprime_bloque(e, bAct);
         }         
         fseek(f,0,SEEK_END);
         pos1=ftell(f);
         *((long*)(b.bloque+b.tam-sizeof(long)*2))=pos1;
         if(compara_bloque(bAct, b, e)>0)
         {            
            pos1=*((long*)(bAnt.bloque+bAnt.tam-sizeof(long)));
            *((long*)(b.bloque+b.tam-sizeof(long)))=pos1;            
            pos1=*((long*)(b.bloque+b.tam-sizeof(long)*2));
            *((long*)(bAnt.bloque+bAnt.tam-sizeof(long)))=pos1;            
            pos1 = *((long*)(bAnt.bloque + bAnt.tam - sizeof(long) * 2));
            fseek(f, pos1, SEEK_SET);
            fwrite(bAnt.bloque, bAnt.tam, 1, f);
         }
         else 
         {
            if(compara_bloque(bAct, b, e)<0)
            {            
               pos1=*((long*)(b.bloque+b.tam-sizeof(long)*2));
               *((long*)(bAct.bloque+bAct.tam-sizeof(long)))=pos1;
               //actualizar bAct en el archivo
               pos1 = *((long*)(bAct.bloque + bAct.tam - sizeof(long) * 2));
               fseek(f, 0, SEEK_SET);
               fseek(f, pos1, SEEK_SET);
               fwrite(bAct.bloque, bAct.tam, 1, f);
               res=1;
            }
         }
      }
      if(res)
      {
         fseek(f, 0, SEEK_END);
         fwrite(b.bloque, b.tam, 1, f);
      }
      else
      {
         fseek(f, pos, SEEK_SET);
         fwrite(b.bloque, b.tam, 1, f);
      }
   }
   fclose(f);
}

void elimina_bloque(TNodoE *ent, TBloque b)
{
  FILE *f;  
  char nom_arch[100];
  long cab=-1L, pos=-1L, pos_ant=-1L,pos1=-1L;
  TBloque bAct={NULL,0}, bAnt={NULL,0};

  strcpy(nom_arch, ent->info.nombre);
  strcat(nom_arch, ".dat");
  f=fopen(nom_arch, "rb+");  
  if(f)
  {
    fread(&cab, sizeof(long), 1, f);
    if(cab!=-1L)
    {
      bAct=crea_bloque(ent);
      fseek(f, cab, SEEK_SET);
      fread(bAct.bloque, bAct.tam, 1, f);
    }
    if(cab==-1L || compara_bloque(bAct, b, ent)==0)
    {
      cab=*((long*)(bAct.bloque+bAct.tam-sizeof(long)));
      fseek(f, 0, SEEK_SET);
      fwrite(&cab, sizeof(long), 1, f);
    }
    else
    {
      while(compara_bloque(bAct, b, ent)!=0 && *((long*)(bAct.bloque+bAct.tam-sizeof(long)))!=-1L)
      {
        if(bAnt.bloque!=NULL)
        {
          free(bAnt.bloque);
        }
        bAnt=bAct;
        bAct=crea_bloque(ent);
        pos_ant=*((long*)(bAnt.bloque+bAnt.tam-sizeof(long)));
        fseek(f, pos_ant, SEEK_SET);
        fread(bAct.bloque, bAct.tam, 1, f);
      }
      if(compara_bloque(bAct, b, ent)==0)
      {
        pos1=*((long*)(bAct.bloque+bAct.tam-sizeof(long)));
        *((long*)(bAnt.bloque+bAnt.tam-sizeof(long)))=pos1;        
        pos1=*((long*)(bAnt.bloque+bAnt.tam-sizeof(long)*2));
        fseek(f, 0, SEEK_SET);
        fseek(f, pos1, SEEK_SET);
        fwrite(bAnt.bloque, bAnt.tam, 1, f);
      }
      else
      {
        printf("\nBloque no existente\n");
      }
    }
  }
  else
  {
    printf("\nEntidad no existente\n");
  }
  fclose(f);
}
//Santillan Salazar Ricardo Adrian 
void imprime_Tabla(TNodoE *e) 
{
  FILE *f;
  char nom_arch[50];
  long cab, dir;
  TBloque b1;
  int band;

  strcpy(nom_arch, e->info.nombre);
  strcat(nom_arch, ".dat");
  printf("nombre del archivo: %s\n", nom_arch);
  f = fopen(nom_arch, "rb+");
  if (f == NULL) 
  {
    printf("Esa tabla no existe\n");
    return;
  }
  fread(&cab, sizeof(long), 1, f);
  b1 = crea_bloque(e);
  while (cab != -1L)
  {
    fseek(f, cab, SEEK_SET);
    fread(b1.bloque, b1.tam, 1, f);
    imprime_bloque(e, b1);
    cab = *((long *)(b1.bloque + b1.tam - (sizeof(long))));
  }
  fclose(f);
}


void imprime_diccionario(char *nom_dic)
{
  TEntidad aux;
  TAtributo aux_a;
  FILE *f;
  long dir,dira;

  f=fopen(nom_dic,"rb");
  if(f)
  {
    fread(&dir,sizeof(long),1,f);
    while(dir!=-1)
    {
       fseek(f,dir,SEEK_SET);
       fread(&aux,sizeof(TEntidad), 1, f);
       imprime_entidad(aux);
       dira=aux.ptr_atrib;
       while(dira!=-1L)
         {
          fseek(f,dira,SEEK_SET);
          fread(&aux_a,sizeof(TAtributo), 1, f);
          imprime_atributo(aux_a);
           dira=aux_a.ptr_sig_a;
         }
       dir=aux.prt_sigent;
    }
    fclose(f);
  }
}

TBloque busca_Bloque(TNodoE *e)
{
  FILE *archivo;
  char nombreArchivo[50];
  long cabecera = -1L, posicion;
  TBloque bloqueActual = {NULL, 0}, encontrado = {NULL, 0};
  TNodoA *atributo = e->cab_atr;
  int desplazamiento = 0;
  int res = 0;
  int valorEntero;
  float valorFlotante;
  char valorChar, valorCadena[100];

  strcpy(nombreArchivo, e->info.nombre);
  strcat(nombreArchivo, ".dat");
  archivo = fopen(nombreArchivo, "rb+");
  if (archivo != NULL)
  {
    while (atributo && atributo->info.clave != 1)
    {
      desplazamiento += atributo->info.tam;
      atributo = atributo->sig;
    }
    printf("%s a buscar: ", atributo->info.nombre);
    switch (atributo->info.tipo)
    {
    case 1:
      scanf("%d", &valorEntero);
      break;
    case 2:
      scanf("%f", &valorFlotante);
      break;
    case 3:
      scanf("%c", &valorChar);
      break;
    case 4:
      scanf("%s", valorCadena);
      break;
    }
    fread(&posicion, sizeof(long), 1, archivo);
    bloqueActual = crea_bloque(e);
    while (posicion != -1L)
    {
      fseek(archivo, posicion, SEEK_SET);
      fread(bloqueActual.bloque, bloqueActual.tam, 1, archivo);
      switch (atributo->info.tipo)
      {
      case 1:
        if (*((int *)(bloqueActual.bloque + desplazamiento)) == valorEntero)
        {
          encontrado = bloqueActual;
          res = 1;
        }
        break;
      case 2:
        if (*((float *)(bloqueActual.bloque + desplazamiento)) == valorFlotante)
        {
          encontrado = bloqueActual;
          res = 1;
        }
        break;
      case 3:
        if (*((char *)(bloqueActual.bloque + desplazamiento)) == valorChar)
        {
          encontrado = bloqueActual;
          res = 1;
        }
        break;
      case 4:
        if (strcmp((char *)(bloqueActual.bloque + desplazamiento), valorCadena) == 0)
        {
          encontrado = bloqueActual;
          res = 1;
        }
        break;
      }
      if (res == 1)
      {
        break;
      }
      posicion = *((long *)(bloqueActual.bloque + bloqueActual.tam - sizeof(long) * 1));
    }
  }
  fclose(archivo);
  return encontrado;
}
