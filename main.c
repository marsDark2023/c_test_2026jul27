#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nvmap.h"

#define PROMPT_0 "||>"
#define PROMPT_1 ">>>"
#define PROMPT_2 "=====\n"
#define MENU_STR "__________\n1.insert; 2.remove; 3.modify; 4.search; 5.statistic; 6.overview; 7.exit;\n^^^^^^^^^^\n"

#define BUFFER_SIZE 64
#define BUFFER_HOLDER "%64s"

#define STATISTIC_TMP_1 "chinese  \t%d\t  %s\n" \
    "math     \t%d\t  %s\n" \
    "english  \t%d\t  %s\n" 
#define STATISTIC_TMP_2 \
    "chinese  \t%0.4f\n" \
    "math     \t%0.4f\n" \
    "english  \t%0.4f\n"
const char* statistic_tmplate = PROMPT_2 \
    "max\n" \
    PROMPT_2 \
    STATISTIC_TMP_1 \
    PROMPT_2 \
    "min\n" \
    PROMPT_2 \
    STATISTIC_TMP_1 \
    PROMPT_2 \
    "average\n" \
    PROMPT_2 \
    STATISTIC_TMP_2 
;
#define SHOW_SUBJECT "name\tchinese\tmath\tenglish\n"
#define SHOW_TMP     "%s\t%d\t%d\t%d\n"


void Op_insert( Nvmap* map ){
    char buffer[BUFFER_SIZE];
    NameVal nv;
    Subject* sub = &(nv.val);
    printf("name"PROMPT_1);
    scanf( BUFFER_HOLDER, buffer );    
    printf("score"PROMPT_1);
    scanf("%d %d %d", &(sub->chinese), &(sub->math), &(sub->english) ); fflush( stdin );
    nv.name = buffer;
    Nvmap_insert( map, &nv );
}
void Op_remove( Nvmap* map ){
    char buffer[BUFFER_SIZE];
    NameVal nv;
    printf("name"PROMPT_1);
    scanf( BUFFER_HOLDER, buffer ); 
    nv.name = buffer;
    if( Nvmap_remove( map, &nv ) )
        printf(PROMPT_2"no one named <%s>\n"PROMPT_2, buffer);
}
void Op_modify( Nvmap* map ){
    char buffer[BUFFER_SIZE];
     NameVal nv;
    Subject* sub = &(nv.val);
    printf("name"PROMPT_1);
    scanf( BUFFER_HOLDER, buffer );
    printf("score"PROMPT_1);
    scanf("%d %d %d", &(sub->chinese), &(sub->math), &(sub->english) );fflush( stdin );
    nv.name = buffer;
    uint32_t status = Nvmap_modify( map, &nv );   
    if( status != 0 )
        printf(PROMPT_2"no one named <%s>\n"PROMPT_2, buffer );
}
void Op_search( Nvmap* map) {
    char buffer[BUFFER_SIZE];
    NameVal nv;
    NameVal* res;
    printf("name"PROMPT_1);
    scanf( BUFFER_HOLDER, buffer );
    nv.name = buffer;
    res = Nvmap_find( map, &nv );
    if( res ){
        printf(PROMPT_2);
        printf("%s\t%d\t%d\t%d\n", 
            res->name, res->val.chinese, 
            res->val.math, res->val.english );
        printf(PROMPT_2);
    }
    else
        printf( PROMPT_2"no one named <%s>\n"PROMPT_2, buffer );
}
void Op_statistic( Nvmap* map ){
    Subject max = {0,0,0};
    char* max_name[] = {0,0,0};
    Subject min = { (uint32_t)-1 , (uint32_t)-1 , (uint32_t)-1 };
    char* min_name[] = {0,0,0};
    Subject total = {0,0,0};
    float num = 0;
    NameVal* nv = 0;
    for(;;++num){
       nv = Nvmap_iterator( map );
       if(!nv)
            break;
        uint32_t ch = nv->val.chinese;
        uint32_t ma = nv->val.math;
        uint32_t en = nv->val.english;
        total.chinese += ch;
        total.math += ma;
        total.english += en;
        if( max.chinese < ch )  { max.chinese = ch; max_name[0] = nv->name; }
        if( max.math    < ma )  { max.math = ma;    max_name[1] = nv->name; }
        if( max.english < en )  { max.english = en; max_name[2] = nv->name; }
        if( min.chinese > ch )  { min.chinese = ch; min_name[0] = nv->name; }
        if( min.math    > ma )  { min.math = ma;    min_name[1] = nv->name; }
        if( min.english > en )  { min.english = en; min_name[2] = nv->name; }
    }
    printf( statistic_tmplate, 
        max.chinese, max_name[0], max.math, max_name[1], max.english, max_name[2],
        min.chinese, min_name[0], min.math, min_name[1], min.english, min_name[2],
        (float)(total.chinese) / num, (float)(total.math) / num, (float)(total.english) / num
    );
}

void Op_show( Nvmap* map ){
    NameVal* ptr = 0;
    printf( SHOW_SUBJECT );
    for(;;){
        ptr = Nvmap_iterator( map );
        if(!ptr)
            break;
        printf( SHOW_TMP, ptr->name, ptr->val.chinese, ptr->val.math, ptr->val.english );
    }
}
void Op_exit( Nvmap* map ){
    exit(0);
}
typedef void(*fun)(Nvmap*);
fun options[] = { Op_insert, Op_remove, Op_modify, Op_search, Op_statistic, Op_show, Op_exit };
int main(){
    Nvmap map;
    Nvmap_init( &map );
    char buf[64];
    uint32_t op = 0;
    for(;;){
        loop:;
        printf(MENU_STR);
        printf(PROMPT_0);
        scanf( "%64s", buf ); fflush(stdin);
        op = atoi( buf );
       if( op>0 && op<8){
            void(*fn)(Nvmap*) = options[op-1] ;
            fn( &map );
       } 
    }
return 0;
}

