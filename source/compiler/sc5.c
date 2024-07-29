/*  Pawn compiler - Error message system
 *  In fact a very simple system, using only 'panic mode'.
 *
 *  Copyright (c) ITB CompuPhase, 1997-2006
 *
 *  This software is provided "as-is", without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1.  The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software in
 *      a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *  2.  Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *  3.  This notice may not be removed or altered from any source distribution.
 *
 *  Version: $Id: sc5.c 3579 2006-06-06 13:35:29Z thiadmer $
 */

#include <assert.h>
#include <wchar.h>
#if defined	__WIN32__ || defined _WIN32 || defined __MSDOS__
  #include <io.h>
#endif
#if defined LINUX || defined __GNUC__
  #include <unistd.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>     /* ANSI standardized variable argument list functions */
#include <string.h>
#if defined FORTIFY
  #include <alloc/fortify.h>
#endif
#include "sc.h"

static wchar_t *errmsg[] = {
/*001*/  L"token esperado: \"%s\", mas encontrado \"%s\"\n",
/*002*/  L"apenas uma única instrução (ou expressão) pode seguir cada \"case\"\n",
/*003*/  L"declaração de uma variável local deve aparecer em um bloco composto\n",
/*004*/  L"função \"%s\" não está implementada\n",
/*005*/  L"a função não pode ter argumentos\n",
/*006*/  L"deve ser atribuído a um array\n",
/*007*/  L"o operador não pode ser redefinido\n",
/*008*/  L"deve ser uma expressão constante; assumido como zero\n",
/*009*/  L"tamanho do array inválido (negativo, zero ou fora dos limites)\n",
/*010*/  L"função ou declaração inválida\n",
/*011*/  L"deslocamento/endereçamento de dados da pilha deve ser um múltiplo do tamanho da célula\n",
/*012*/  L"chamada de função inválida, não é um endereço válido\n",
/*013*/  L"nenhum ponto de entrada (nenhuma função pública)\n",
/*014*/  L"instrução inválida; não está no switch\n",
/*015*/  L"o caso \"default\" deve ser o último caso na instrução switch\n",
/*016*/  L"múltiplos defaults no \"switch\"\n",
/*017*/  L"símbolo indefinido \"%s\"\n",
/*018*/  L"dados de inicialização excedem o tamanho declarado\n",
/*019*/  L"não é um rótulo: \"%s\"\n",
/*020*/  L"nome de símbolo inválido \"%s\"\n",
/*021*/  L"símbolo já definido: \"%s\"\n",
/*022*/  L"deve ser um lvalue (não constante)\n",
/*023*/  L"atribuição de array deve ser uma atribuição simples\n",
/*024*/  L"\"break\" ou \"continue\" está fora de contexto\n",
/*025*/  L"cabeça da função difere do protótipo\n",
/*026*/  L"nenhum \"#if...\" correspondente\n",
/*027*/  L"constante de caractere inválida\n",
/*028*/  L"subscrito inválido (não é um array ou muitos subscritos): \"%s\"\n",
/*029*/  L"expressão inválida, assumido como zero\n",
/*030*/  L"declaração composta não fechada no final do arquivo (começou na linha %d)\n",
/*031*/  L"diretiva desconhecida\n",
/*032*/  L"índice de array fora dos limites (variável \"%s\")\n",
/*033*/  L"array deve ser indexado (variável \"%s\")\n",
/*034*/  L"argumento não tem um valor padrão (argumento %d)\n",
/*035*/  L"incompatibilidade de tipo de argumento (argumento %d)\n",
/*036*/  L"declaração vazia\n",
/*037*/  L"string inválida (possivelmente string não terminada)\n",
/*038*/  L"caracteres extras na linha\n",
/*039*/  L"símbolo constante não tem tamanho\n",
/*040*/  L"rótulo \"case\" duplicado (valor %d)\n",
/*041*/  L"ellipsis inválido, tamanho do array não é conhecido\n",
/*042*/  L"combinação inválida de especificadores de classe\n",
/*043*/  L"constante de caractere excede o intervalo para string compactada\n",
/*044*/  L"parâmetros posicionais devem preceder todos os parâmetros nomeados\n",
/*045*/  L"muitos argumentos de função\n",
/*046*/  L"tamanho do array desconhecido (variável \"%s\")\n",
/*047*/  L"tamanhos de array não correspondem, ou o array de destino é muito pequeno\n",
/*048*/  L"dimensões do array não correspondem\n",
/*049*/  L"continuação de linha inválida\n",
/*050*/  L"intervalo inválido\n",
/*051*/  L"subscrito inválido, use operadores \"[ ]\" nas dimensões principais\n",
/*052*/  L"arrays multidimensionais devem ser totalmente inicializados\n",
/*053*/  L"excedendo o número máximo de dimensões\n",
/*054*/  L"chave de fechamento não correspondente (\"}\")\n",
/*055*/  L"início do corpo da função sem cabeçalho de função\n",
/*056*/  L"arrays, variáveis locais e argumentos de função não podem ser públicos (variável \"%s\")\n",
/*057*/  L"expressão inacabada antes da diretiva do compilador\n",
/*058*/  L"argumento duplicado; o mesmo argumento é passado duas vezes\n",
/*059*/  L"argumento da função não pode ter um valor padrão (variável \"%s\")\n",
/*060*/  L"múltiplas diretivas \"#else\" entre \"#if ... #endif\"\n",
/*061*/  L"diretiva \"#elseif\" segue uma diretiva \"#else\"\n",
/*062*/  L"número de operandos não se ajusta ao operador\n",
/*063*/  L"tag de resultado da função do operador \"%s\" deve ser \"%s\"\n",
/*064*/  L"não é possível alterar operadores pré-definidos\n",
/*065*/  L"argumento da função pode ter apenas uma única tag (argumento %d)\n",
/*066*/  L"argumento da função não pode ser um argumento por referência ou um array (argumento \"%s\")\n",
/*067*/  L"variável não pode ser tanto uma referência quanto um array (variável \"%s\")\n",
/*068*/  L"precisão de número racional inválida em #pragma\n",
/*069*/  L"formato de número racional já definido\n",
/*070*/  L"suporte a número racional não foi habilitado\n",
/*071*/  L"operador definido pelo usuário deve ser declarado antes do uso (função \"%s\")\n",
/*072*/  L"operador \"sizeof\" é inválido em símbolos \"function\"\n",
/*073*/  L"argumento da função deve ser um array (argumento \"%s\")\n",
/*074*/  L"padrão #define deve começar com um caractere alfabético\n",
/*075*/  L"linha de entrada muito longa (após substituições)\n",
/*076*/  L"erro de sintaxe na expressão, ou chamada de função inválida\n",
/*077*/  L"codificação UTF-8 malformada, ou arquivo corrompido: %s\n",
/*078*/  L"função usa tanto \"return\" quanto \"return <valor>\"\n",
/*079*/  L"tipos de retorno inconsistentes (array & não-array)\n",
/*080*/  L"símbolo desconhecido, ou não é um símbolo constante (símbolo \"%s\")\n",
/*081*/  L"não é possível usar uma tag como valor padrão para um parâmetro de array indexado (símbolo \"%s\")\n",
/*082*/  L"operadores definidos pelo usuário e funções nativas não podem ter estados\n",
/*083*/  L"uma função ou variável pode pertencer a um único autômato (símbolo \"%s\")\n",
/*084*/  L"conflito de estados: um dos estados já está atribuído a outra implementação (símbolo \"%s\")\n",
/*085*/  L"nenhum estado definido para o símbolo \"%s\"\n",
/*086*/  L"autômato desconhecido \"%s\"\n",
/*087*/  L"estado desconhecido \"%s\" para o autômato \"%s\"\n",
/*088*/  L"variáveis públicas e variáveis locais não podem ter estados (símbolo \"%s\")\n",
/*089*/  L"variáveis de estado não podem ser inicializadas (símbolo \"%s\")\n",
/*090*/  L"funções públicas não podem retornar arrays (símbolo \"%s\")\n",
/*091*/  L"constante ambígua; substituição de tag é necessária (símbolo \"%s\")\n",
/*092*/  L"funções não podem retornar arrays de tamanho desconhecido (símbolo \"%s\")\n",
/*093*/  L"operador \"__addressof\" é inválido em expressões de pré-processador\n"
};

static wchar_t *fatalmsg[] = {
/*100*/  L"não é possível ler do arquivo: \"%s\"\n",
/*101*/  L"não é possível gravar no arquivo: \"%s\"\n",
/*102*/  L"transbordamento da tabela: \"%s\"\n",
          /* a tabela pode ser: tabela de loop
           *               tabela de literais
           *               buffer de estágio
           *               tabela de opções (arquivo de resposta)
           *               tabela de otimizador peephole
           */
/*103*/  L"memória insuficiente\n",
/*104*/  L"instrução de assembler inválida \"%s\"\n",
/*105*/  L"transbordamento numérico, excedendo a capacidade\n",
/*106*/  L"script compilado excede o tamanho máximo de memória (%ld bytes)\n",
/*107*/  L"muitos mensagens de erro em uma linha\n",
/*108*/  L"arquivo de mapeamento de página de código não encontrado\n",
/*109*/  L"caminho inválido: \"%s\"\n",
/*110*/  L"falha na asserção: %s\n",
/*111*/  L"erro do usuário: %s\n"
};

static wchar_t *warnmsg[] = {
/*200*/  L"símbolo \"%s\" foi truncado para %d caracteres\n",
/*201*/  L"redefinição de constante/macros (símbolo \"%s\")\n",
/*202*/  L"número de argumentos não corresponde à definição\n",
/*203*/  L"símbolo nunca é usado: \"%s\"\n",
/*204*/  L"símbolo recebe um valor que nunca é usado: \"%s\"\n",
/*205*/  L"código redundante: expressão constante é zero\n",
/*206*/  L"teste redundante: expressão constante é diferente de zero\n",
/*207*/  L"#pragma desconhecido\n",
/*208*/  L"função com tag de resultado usada antes da definição, forçando reanálise\n",
/*209*/  L"função \"%s\" deve retornar um valor\n",
/*210*/  L"uso possível de símbolo antes da inicialização: \"%s\"\n",
/*211*/  L"atribuição possivelmente não intencional\n",
/*212*/  L"operação bit a bit possivelmente não intencional\n",
/*213*/  L"incompatibilidade de tags: esperado %s %s mas encontrado %s\n",
/*214*/  L"possivelmente um argumento de array \"const\" foi pretendido: \"%s\"\n",
/*215*/  L"expressão não tem efeito\n",
/*216*/  L"comentário aninhado\n",
/*217*/  L"indentação solta\n",
/*218*/  L"protótipos de estilo antigo usados com pontos e vírgulas opcionais\n",
/*219*/  L"variável local \"%s\" oculta uma variável em um nível anterior\n",
/*220*/  L"expressão com substituição de tag deve aparecer entre parênteses\n",
/*221*/  L"nome de rótulo \"%s\" oculta o nome da tag\n",
/*222*/  L"número de dígitos excede a precisão do número racional\n",
/*223*/  L"redundante \"sizeof\": tamanho do argumento é sempre 1 (símbolo \"%s\")\n",
/*224*/  L"tamanho de array indeterminado na expressão \"sizeof\" (símbolo \"%s\")\n",
/*225*/  L"código inacessível\n",
/*226*/  L"uma variável é atribuída a ela mesma (símbolo \"%s\")\n",
/*227*/  L"mais inicializadores do que campos de enumeração\n",
/*228*/  L"comprimento do inicializador excede o tamanho do campo de enumeração\n",
/*229*/  L"incompatibilidade de tag de índice (símbolo \"%s\"): tag esperada %s mas encontrada %s\n",
/*230*/  L"nenhuma implementação para o estado \"%s\" na função \"%s\", sem fallback\n",
/*231*/  L"especificação de estado na declaração antecipada é ignorada\n",
/*232*/  L"arquivo de saída é gravado, mas com codificação compacta desativada\n",
/*233*/  L"variável de estado \"%s\" oculta uma variável global\n",
/*234*/  L"função está obsoleta (símbolo \"%s\") %s\n",
/*235*/  L"função pública carece de declaração antecipada (símbolo \"%s\")\n",
/*236*/  L"parâmetro desconhecido na substituição (padrão #define incorreto)\n",
/*237*/  L"aviso do usuário: %s\n",
/*238*/  L"combinação sem sentido de especificadores de classe (%s)\n",
/*239*/  L"array/string literal passado para um parâmetro não-const\n"
};

static wchar_t *noticemsg[] = {
/*001*/  L"; você quis dizer \"%s\"?\n",
/*002*/  L"; variável de estado fora do escopo\n"
};

#define NUM_WARNINGS    (sizeof warnmsg / sizeof warnmsg[0])
static struct s_warnstack {
  unsigned char disable[(NUM_WARNINGS + 7) / 8]; /* 8 flags in a char */
  struct s_warnstack *next;
} warnstack;

static int errflag;
static int errstart;    /* line number at which the instruction started */
static int errline;     /* forced line number for the error message */
static int errwarn;

/*  error
 *
 *  Outputs an error message (note: msg is passed optionally).
 *  If an error is found, the variable "errflag" is set and subsequent
 *  errors are ignored until lex() finds a semicolumn or a keyword
 *  (lex() resets "errflag" in that case).
 *
 *  Global references: inpfname   (reffered to only)
 *                     fline      (reffered to only)
 *                     fcurrent   (reffered to only)
 *                     errflag    (altered)
 */
SC_FUNC int error(long number,...)
{
static wchar_t *prefix[3]={ L"erro", L"erro fatal", L"aviso" };
static int lastline,errorcount;
static short lastfile;
  wchar_t *msg,*pre;
  va_list argptr;
  wchar_t string[128];
  int notice;

  /* split the error field between the real error/warning number and an optional
   * "notice" number
   */
  notice=(unsigned long)number >> (sizeof(long)*4);
  number&=(~(unsigned long)0) >> (sizeof(long)*4);
  assert(number>0 && number<300);

  /* errflag is reset on each semicolon.
   * In a two-pass compiler, an error should not be reported twice. Therefore
   * the error reporting is enabled only in the second pass (and only when
   * actually producing output). Fatal errors may never be ignored.
   */
  if ((errflag || sc_status!=statWRITE) && (number<100 || number>=200))
    return 0;

  /* also check for disabled warnings */
  if (number>=200) {
    int index=(number-200)/8;
    int mask=1 << ((number-200)%8);
    if ((warnstack.disable[index] & mask)!=0)
      return 0;
  } /* if */

  if (number<100) {
    assert(number>0 && number<(1+sizeof(errmsg)/sizeof(errmsg[0])));
    msg=errmsg[number-1];
    pre=prefix[0];
    errflag=TRUE;       /* set errflag (skip rest of erroneous expression) */
    errnum++;
  } else if (number<200) {
    assert(number>=100 && number<(100+sizeof(fatalmsg)/sizeof(fatalmsg[0])));
    msg=fatalmsg[number-100];
    pre=prefix[1];
    errnum++;           /* a fatal error also counts as an error */
  } else if (errwarn) {
    assert(number>=200 && number<(200+sizeof(warnmsg)/sizeof(warnmsg[0])));
    msg=warnmsg[number-200];
    pre=prefix[0];
    errflag=TRUE;
    errnum++;
  } else {
    assert(number>=200 && number<(200+sizeof(warnmsg)/sizeof(warnmsg[0])));
    msg=warnmsg[number-200];
    pre=prefix[2];
    warnnum++;
  } /* if */

  if (notice!=0) {
    assert(notice>0 && notice<(1+sizeof(noticemsg)/sizeof(noticemsg[0])) && noticemsg[notice-1][0]!='\0');
    wcscpy(string,msg);
    wcscpy(&string[wcslen(string)-1],noticemsg[notice-1]);
    msg=string;
  } /* if */

  assert(errstart<=fline);
  if (errline>0)
    errstart=errline;
  else
    errline=fline;
  assert(errstart<=errline);
  va_start(argptr,number);
  if (errfname[0]=='\0') {
    int start=(errstart==errline) ? -1 : errstart;
    if (pc_werror((int)number,msg,inpfname,start,errline,argptr)) {
      if (outf!=NULL) {
        pc_closeasm(outf,TRUE);
        outf=NULL;
      } /* if */
      longjmp(errbuf,3);        /* user abort */
    } /* if */
  } else {
    FILE *fp=fopen(errfname,"a");
    if (fp!=NULL) {
      if (errstart>=0 && errstart!=errline)
        fwprintf(fp,L"%s(%d -- %d) : %ls %03d: ",inpfname,errstart,errline,pre,(int)number);
      else
        fwprintf(fp,L"%s(%d) : %ls %03d: ",inpfname,errline,pre,(int)number);
      vfwprintf(fp,msg,argptr);
      fclose(fp);
    } /* if */
  } /* if */
  va_end(argptr);

  if ((number>=100 && number<200) || errnum>25){
    if (errfname[0]=='\0') {
      va_start(argptr,number);
      pc_werror(0, L"\nCompilação abortada.\n\n", NULL, 0, 0, argptr);
      va_end(argptr);
    } /* if */
    if (outf!=NULL) {
      pc_closeasm(outf,TRUE);
      outf=NULL;
    } /* if */
    longjmp(errbuf,2);          /* fatal error, quit */
  } /* if */

  errline=-1;
  /* check whether we are seeing many errors on the same line */
  if ((errstart<0 && lastline!=fline) || lastline<errstart || lastline>fline || fcurrent!=lastfile)
    errorcount=0;
  lastline=fline;
  lastfile=fcurrent;
  if (number<200 || errwarn)
    errorcount++;
  if (errorcount>=3)
    error(107);         /* too many error/warning messages on one line */

  return 0;
}

SC_FUNC void errorset(int code,int line)
{
  switch (code) {
  case sRESET:
    errflag=FALSE;      /* start reporting errors */
    break;
  case sFORCESET:
    errflag=TRUE;       /* stop reporting errors */
    break;
  case sEXPRMARK:
    errstart=fline;     /* save start line number */
    break;
  case sEXPRRELEASE:
    errstart=-1;        /* forget start line number */
    errline=-1;
    break;
  case sSETPOS:
    errline=line;
    break;
  } /* switch */
}

/* pc_enablewarning()
 * Enables or disables a warning (errors cannot be disabled).
 * Initially all warnings are enabled. The compiler does this by setting bits
 * for the *disabled* warnings and relying on the array to be zero-initialized.
 *
 * Parameter enable can be:
 *  o  0 for disable
 *  o  1 for enable
 *  o  2 for toggle
 */
int pc_enablewarning(int number,int enable)
{
  int index;
  unsigned char mask;

  if (number<200)
    return FALSE;       /* errors and fatal errors cannot be disabled */
  number-=200;
  if (number>=NUM_WARNINGS)
    return FALSE;

  index=number/8;
  mask=(unsigned char)(1 << (number%8));
  switch (enable) {
  case 0:
    warnstack.disable[index] |= mask;
    break;
  case 1:
    warnstack.disable[index] &= (unsigned char)~mask;
    break;
  case 2:
    warnstack.disable[index] ^= mask;
    break;
  } /* switch */

  return TRUE;
}

/* pc_pushwarnings()
 * Saves currently disabled warnings, used to implement #pragma warning push
 */
int pc_pushwarnings()
{
  void *p;
  p=calloc(sizeof(struct s_warnstack),1);
  if (p==NULL) {
    error(103); /* insufficient memory */
    return FALSE;
  }
  memmove(p,&warnstack,sizeof(struct s_warnstack));
  warnstack.next=p;
  return TRUE;
}

/* pc_popwarnings()
 * This function is the reverse of pc_pushwarnings()
 */
int pc_popwarnings()
{
  void *p;
  if (warnstack.next==NULL)
    return FALSE; /* nothing to do */
  p=warnstack.next;
  memmove(&warnstack,p,sizeof(struct s_warnstack));
  free(p);
  return TRUE;
}

/* pc_seterrorwarnings()
 * Make warnings errors (or not).
 */
void pc_seterrorwarnings(int enable)
{
  errwarn = enable;
}

int pc_geterrorwarnings()
{
  return errwarn;
}

/* Implementation of Levenshtein distance, by Lorenzo Seidenari
 */
static int minimum(int a,int b,int c)
{
  int min=a;
  if(b<min)
    min=b;
  if(c<min)
    min=c;
  return min;
}

static int levenshtein_distance(const char *s,const char*t)
{
  //Step 1
  int k,i,j,cost,*d,distance;
  int n=strlen(s);
  int m=strlen(t);
  assert(n>0 && m>0);
  d=(int*)malloc((sizeof(int))*(m+1)*(n+1));
  m++;
  n++;
  //Step 2
  for (k=0;k<n;k++)
    d[k]=k;
  for (k=0;k<m;k++)
    d[k*n]=k;
  //Step 3 and 4
  for (i=1;i<n;i++) {
    for (j=1;j<m;j++) {
      //Step 5
      cost= (tolower(s[i-1])!=tolower(t[j-1]));
      //Step 6
      d[j*n+i]=minimum(d[(j-1)*n+i]+1,d[j*n+i-1]+1,d[(j-1)*n+i-1]+cost);
    } /* for */
  } /* for */
  distance=d[n*m-1];
  free(d);
  return distance;
}

static int get_max_dist(const char *name)
{
  int max_dist=strlen(name)/2; /* for short names, allow only a single edit */
  if (max_dist>MAX_EDIT_DIST)
    max_dist=MAX_EDIT_DIST;
  return max_dist;
}

static int find_closest_symbol_table(const char *name,const symbol *root,int symboltype,symbol **closest_sym)
{
  int dist,max_dist,closest_dist=INT_MAX;
  char symname[2*sNAMEMAX+16];
  symbol *sym;
  assert(closest_sym!=NULL);
  *closest_sym =NULL;
  assert(name!=NULL);
  max_dist=get_max_dist(name);
  for (sym=root->next; sym!=NULL; sym=sym->next) {
    if (sym->fnumber!=-1 && sym->fnumber!=fcurrent)
      continue;
    if ((sym->usage & uDEFINE)==0 && (sym->ident!=iFUNCTN || (sym->usage & (uNATIVE | uPROTOTYPED))!=uPROTOTYPED))
      continue;
    switch (sym->ident)
    {
    case iLABEL:
      if ((symboltype & esfLABEL)==0)
        continue;
      break;
    case iCONSTEXPR:
      if ((symboltype & esfCONST)==0)
        continue;
      break;
    case iVARIABLE:
    case iREFERENCE:
      if ((symboltype & esfVARIABLE)==0)
        continue;
      break;
    case iARRAY:
    case iREFARRAY:
      if ((symboltype & esfARRAY)==0)
        continue;
      break;
    case iFUNCTN:
    case iREFFUNC:
      if ((symboltype & (((sym->usage & uNATIVE)!=0) ? esfNATIVE : esfFUNCTION))==0)
        continue;
      break;
    default:
      assert(0);
    } /* switch */
    funcdisplayname(symname,sym->name);
    if (symname[0] == '\0')
      continue;
    dist=levenshtein_distance(name,symname);
    if (dist>max_dist || dist>=closest_dist)
      continue;
    *closest_sym=sym;
    closest_dist=dist;
    if (closest_dist<=1)
      break;
  } /* for */
  return closest_dist;
}

static symbol *find_closest_symbol(const char *name,int symboltype)
{
  symbol *symloc,*symglb;
  int distloc,distglb;

  if (sc_status==statFIRST)
    return NULL;
  assert(name!=NULL);
  if (name[0]=='\0')
    return NULL;
  distloc=find_closest_symbol_table(name,&loctab,symboltype,&symloc);
  if (distloc<=1)
    distglb=INT_MAX; /* don't bother searching in the global table */
  else
    distglb=find_closest_symbol_table(name,&glbtab,symboltype,&symglb);
  return (distglb<distloc) ? symglb : symloc;
}

static constvalue *find_closest_automaton(const char *name)
{
  constvalue *ptr=sc_automaton_tab.first;
  constvalue *closest_match=NULL;
  int dist,max_dist,closest_dist=INT_MAX;

  assert(name!=NULL);
  max_dist=get_max_dist(name);
  while (ptr!=NULL) {
    if (ptr->name[0]!='\0') {
      dist=levenshtein_distance(name,ptr->name);
      if (dist<closest_dist && dist<=max_dist) {
        closest_match=ptr;
        closest_dist=dist;
        if (closest_dist<=1)
          break;
      } /* if */
    } /* if */
    ptr=ptr->next;
  } /* while */
  return closest_match;
}

static constvalue *find_closest_state(const char *name,int fsa)
{
  constvalue *ptr=sc_state_tab.first;
  constvalue *closest_match=NULL;
  int dist,max_dist,closest_dist=INT_MAX;

  assert(name!=NULL);
  max_dist=get_max_dist(name);
  while (ptr!=NULL) {
    if (ptr->index==fsa && ptr->name[0]!='\0') {
      dist=levenshtein_distance(name,ptr->name);
      if (dist<closest_dist && dist<=max_dist) {
        closest_match=ptr;
        closest_dist=dist;
        if (closest_dist<=1)
          break;
      } /* if */
    } /* if */
    ptr=ptr->next;
  } /* while */
  return closest_match;
}

static constvalue *find_closest_automaton_for_state(const char *statename,int fsa)
{
  constvalue *ptr=sc_state_tab.first;
  constvalue *closest_match=NULL;
  constvalue *automaton;
  const char *fsaname;
  int dist,max_dist,closest_dist=INT_MAX;

  assert(statename!=NULL);
  max_dist=get_max_dist(statename);
  automaton=automaton_findid(ptr->index);
  assert(automaton!=NULL);
  fsaname=automaton->name;
  while (ptr!=NULL) {
    if (fsa!=ptr->index && ptr->name[0]!='\0' && strcmp(statename,ptr->name)==0) {
      automaton=automaton_findid(ptr->index);
      assert(automaton!=NULL);
      dist=levenshtein_distance(fsaname,automaton->name);
      if (dist<closest_dist && dist<=max_dist) {
        closest_match=automaton;
        closest_dist=dist;
        if (closest_dist<=1)
          break;
      } /* if */
    } /* if */
    ptr=ptr->next;
  } /* while */
  return closest_match;
}

SC_FUNC int error_suggest(int number,const char *name,const char *name2,int type,int subtype)
{
  char string[sNAMEMAX*2+2]; /* for "<automaton>:<state>" */
  const char *closest_name=NULL;
  symbol *closest_sym;

  /* don't bother finding the closest names on errors
   * that aren't going to be shown on the 1'st pass
   */
  if ((errflag || sc_status!=statWRITE) && (number<100 || number>=200))
    return 0;

  if (type==estSYMBOL) {
  find_symbol:
    closest_sym=find_closest_symbol(name,subtype);
    if (closest_sym!=NULL) {
      closest_name=closest_sym->name;
      if ((subtype & esfVARIABLE)!=0 && closest_sym->states!=NULL && strcmp(closest_name,name)==0) {
        assert(number==17); /* undefined symbol */
        error(makelong(number,2),name);
        return 0;
      } /* if */
    } /* if */
  } else if (type==estNONSYMBOL) {
    if (tMIDDLE<subtype && subtype<=tLAST) {
      extern char *sc_tokens[];
      name=sc_tokens[subtype-tFIRST];
      subtype=esfVARCONST;
      goto find_symbol;
    } /* if */
  } else if (type==estAUTOMATON) {
    constvalue *closest_automaton=find_closest_automaton(name);
    if (closest_automaton!=NULL)
      closest_name=closest_automaton->name;
  } else if (type==estSTATE) {
    constvalue *closest_state=find_closest_state(name,subtype);
    if (closest_state!=NULL) {
      closest_name=closest_state->name;
    } else {
      constvalue *closest_automaton=find_closest_automaton_for_state(name,subtype);
      if (closest_automaton!=NULL) {
        sprintf(string,"%s:%s",closest_automaton->name,name);
        closest_name=string;
      } /* if */
    } /* if */
  } else {
    assert(0);
  } /* if */

  if (closest_name==NULL) {
    error(number,name,name2);
  } else if (name2!=NULL) {
    error(makelong(number,1),name,name2,closest_name);
  } else {
    error(makelong(number,1),name,closest_name);
  } /* if */
  return 0;
}
