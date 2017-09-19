# ########################################################
# Importation de packages
# ########################################################

from tkinter import *
import os
import math
import time # pour avoir le temsp de calcul cpu
#from numpy import *
import copy

# ########################################################
# Lecture du fichier
# ########################################################

mesdonnees = open("input.txt","r")
ligne1 = mesdonnees.readline()
indice=0
j=0
while j <= len(ligne1) and ligne1[j]!= '\n':
    val = ''
    while (ligne1[j] == ' ') and j <= len(ligne1) and ligne1[j]!= '\n':
        j=j+1
    indice=indice+1
    while(ligne1[j] != ' ') and j <= len(ligne1) and ligne1[j]!= '\n':
        val = val+ligne1[j]
        j=j+1
    #print('val=',val)
    if indice == 1: m=int(val)
    elif indice ==2: nbjobs=int(val)
#print('nbjobs=',nbjobs,' m=',m)

pp=[[] for i in range(nbjobs)]
for i in range(0,m):
    ligne = mesdonnees.readline()
    laligne = ligne.split('\t')
    #print(laligne)
    if laligne != '\n':
        for j in range(0,nbjobs):
            val = int(laligne[j])
            #print('val=',val)
            pp[j].insert(i,val)
#print('pp=',pp)

dd=[]
ligne = mesdonnees.readline()
laligne = ligne.split('\t')
for j in range(0,nbjobs):
    val = int(laligne[j])
    #print('val=',val)
    dd.append(val)
#print('dd=',dd)

tt=[[] for i in range(nbjobs+1)]
for i in range(0,nbjobs+1):
    ligne = mesdonnees.readline()
    laligne = ligne.split('\t')
    #print(laligne)
    if laligne != '\n':
        for j in range(0,nbjobs+1):
            val = int(laligne[j])
            #print('val=',val)
            tt[j].insert(i,val)
#print('tt=',tt)

mesdonnees.close()


# ===================================================
# Constantes
# ===================================================

nbjobs=len(pp)
INFINI = 999999999
ListeTabou=[]
TailleTabou = 7
TIME_LIMIT = nbjobs * m / 4

if nbjobs == 100: DELTA = 8
elif nbjobs == 50: DELTA = 10
else: DELTA = nbjobs/4

DELTA_EBFSR_SOLO = 3

NB_ITE_MAX = 2000
if nbjobs == 100: NB_ITE_SANS_AMEL_MAX = 5
elif nbjobs == 50: NB_ITE_SANS_AMEL_MAX = 8
else: NB_ITE_SANS_AMEL_MAX = 12
AVEC_DIVERSIFICATION = 1

FLAG_SWAP_SEQ = 1
FLAG_SWAP_BATCH = 1
FLAG_SWAP_BOTH = 1
FLAG_EBSR_BOTH = 1
FLAG_EBSR_SEQ = 1
FLAG_EBSR_BATCH = 1
FLAG_EFSR_BOTH = 1
FLAG_EFSR_SEQ = 1
FLAG_EFSR_BATCH = 1
FLAG_2OPT = 1

FLAG_FIRST_IMPROVE = 1
TABOU_LOGIQUE = 0 # je swappe(i,j) donc (j,i) est tabou

def InsereTabou(voisinage,indi,indj):
    ElemTabou=[voisinage,indi,indj]
    if len(ListeTabou) == TailleTabou:
        del(ListeTabou[0])
    ListeTabou.append(ElemTabou)
    #print('ListeTabou :',ListeTabou)

def PasTabou(voisinage,indi,indj):
    ElemTabou=[voisinage,indi,indj]
    pasTabou = True
    if(ElemTabou in ListeTabou):
        #print('Element ',ElemTabou,' dans la liste Tabou')
        pasTabou = False
    return(pasTabou)

# ===================================================
# Voisinages
# ===================================================

def swap_both(i,j,sol,div):
    # la sequence d'entree est modifiee par swap_seq
    #==================================================================
    # le swap est effectue dans la sequence et dans le batch
    #==================================================================
    #print('avant swap_seq:',i,j,sol)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    sol[0]=sol[0][0:i]+job_2+sol[0][i+1:j]+job_1+sol[0][j+1:nbjobs]
    u=0
    u_i,u_j=-1,-1
    while u <= len(sol[1]) and (u_i == -1 or u_j == -1):
        if job_i in sol[1][u]:
            index_i = sol[1][u].index(job_i)
            u_i =u
            #print('i=',i,' indice ',index_i,'tournee ',u)
        if job_j in sol[1][u]:
            index_j = sol[1][u].index(job_j)
            u_j =u
            #print('j=',j,' indice ',index_j,'tournee ',u)
        u=u+1
    sol[1][u_i][index_i]=job_j
    sol[1][u_j][index_j]=job_i
    #print('apres swap_both:',i,j,sol)
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    return (valeur)


def swap_seq(i,j,sol,div):
    # la sequence d'entree est modifiee par swap_seq
    #==================================================================
    # le swap est effectue dans la sequence
    #==================================================================
    #print('avant swap_seq:',i,j,sol)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    sol[0]=sol[0][0:i]+job_2+sol[0][i+1:j]+job_1+sol[0][j+1:nbjobs]
    #print('apres swap_seq:',i,j,sol)
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    return (valeur)


def swap_batch(i,j,sol,div):
    # la sequence d'entree est modifiee par swap_batch
    #==================================================================
    # le swap est effectue dans le batch
    #==================================================================
    #print('avant swap_batch:',i,j,sol)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    u=0
    u_i,u_j=-1,-1
    while u <= len(sol[1]) and (u_i == -1 or u_j == -1):
        if job_i in sol[1][u]:
            index_i = sol[1][u].index(job_i)
            u_i =u
            #print('i=',i,' indice ',index_i,'tournee ',u)
        if job_j in sol[1][u]:
            index_j = sol[1][u].index(job_j)
            u_j =u
            #print('j=',j,' indice ',index_j,'tournee ',u)
        u=u+1
    sol[1][u_i][index_i]=job_j
    sol[1][u_j][index_j]=job_i
    #print('apres swap_batch:',i,j,sol)
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    return (valeur)

def ebsr_both(i,j,sol,div):
    # la sequence d'entree n'est pas modifiee par ebsr
    #==================================================================
    # l'ebsr est effectue dans la sequence
    #==================================================================
    #print('avant ebsr_seq:',sol,i,j)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    sol[0]=sol[0][0:i]+job_2+sol[0][i:j]+sol[0][j+1:nbjobs]
    u=0
    u_i,u_j=-1,-1
    while u <= len(sol[1]) and (u_i == -1 or u_j == -1):
        if job_i in sol[1][u]:
            index_i = sol[1][u].index(job_i)
            u_i =u
        if job_j in sol[1][u]:
            index_j = sol[1][u].index(job_j)
            u_j =u
        u=u+1
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    #print('apres ebsr_seq:',sol,i,j)
    return (valeur)

def ebsr_seq(i,j,sol,div):
    # la sequence d'entree n'est pas modifiee par ebsr
    #==================================================================
    # l'ebsr est effectue dans la sequence
    #==================================================================
    #print('avant ebsr_seq:',sol,i,j)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    sol[0]=sol[0][0:i]+job_2+sol[0][i:j]+sol[0][j+1:nbjobs]
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    #print('apres ebsr_seq:',sol,i,j)
    return (valeur)

def ebsr_batch(i,j,sol,div):
    # la sequence d'entree n'est pas modifiee par ebsr
    #==================================================================
    # l'ebsr est effectue dans le batch
    #==================================================================
    #print('avant ebsr:',sol,i,j)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    u=0
    u_i,u_j=-1,-1
    while u <= len(sol[1]) and (u_i == -1 or u_j == -1):
        if job_i in sol[1][u]:
            index_i = sol[1][u].index(job_i)
            u_i =u
        if job_j in sol[1][u]:
            index_j = sol[1][u].index(job_j)
            u_j =u
        u=u+1
    #print('u_i=',u_i,' u_j=',u_j,' index_i=',index_i,' index_j=',index_j)
    del sol[1][u_j][index_j]
    sol[1][u_i].insert(index_i-1,job_j)
    if (sol[1][u_j] == []): del sol[1][u_j]
    #print(sol)
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    #print('apres ebsr:',sol,i,j)
    return (valeur)

def efsr_both(i,j,sol,div):
    # la sequence d'entree n'est pas modifiee par efsr
    #==================================================================
    # l'efsr est effectue dans la sequence et dans le batch
    #==================================================================
    #print('avant efsr_seq:',sol,i,j)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    sol[0]=sol[0][0:i]+sol[0][i+1:j]+job_1+sol[0][j:nbjobs]
    u=0
    u_i,u_j=-1,-1
    while u <= len(sol[1]) and (u_i == -1 or u_j == -1):
        if job_i in sol[1][u]:
            index_i = sol[1][u].index(job_i)
            u_i =u
        if job_j in sol[1][u]:
            index_j = sol[1][u].index(job_j)
            u_j =u
        u=u+1
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    #print('apres efsr_seq:',sol,i,j)
    return (valeur)

def efsr_seq(i,j,sol,div):
    # la sequence d'entree n'est pas modifiee par efsr
    #==================================================================
    # l'efsr est effectue dans la sequence
    #==================================================================
    #print('avant efsr_seq:',sol,i,j)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    sol[0]=sol[0][0:i]+sol[0][i+1:j]+job_1+sol[0][j:nbjobs]
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    #print('apres efsr_seq:',sol,i,j)
    return (valeur)


def efsr_batch(i,j,sol,div):
    # la sequence d'entree n'est pas modifiee par efsr
    #==================================================================
    # l'efsr est effectue dans le batch
    #==================================================================
    #print('avant efsr_batch:',sol,i,j)
    job_i = sol[0][i]
    job_1 = [job_i]
    job_j = sol[0][j]
    job_2 = [job_j]
    u=0
    u_i,u_j=-1,-1
    while u <= len(sol[1]) and (u_i == -1 or u_j == -1):
        if job_i in sol[1][u]:
            index_i = sol[1][u].index(job_i)
            u_i =u
        if job_j in sol[1][u]:
            index_j = sol[1][u].index(job_j)
            u_j =u
        u=u+1
    #print('u_i=',u_i,' u_j=',u_j,' index_i=',index_i,' index_j=',index_j)
    del sol[1][u_i][index_i]
    sol[1][u_j].insert(index_j-1,job_i)
    if (sol[1][u_i] == []): del sol[1][u_i]
    #print(sol)
    valeur = evalue(sol,div)
##    if not div:
##        valeur = evalue(sol)
##    else:
##        valeur = evalue_div(sol)
    #print('apres efsr_batch:',sol,i,j)
    return (valeur)


# ===================================================
# Evaluation d'une solution
# ===================================================
def evalue(sol,div):
    Cjm=[[0 for i in range(m)] for i in range (nbjobs)]
    Cm=[0]*nbjobs
    fake_dd = copy.deepcopy(dd)
    for j in range(0,nbjobs):
        jobj= sol[0][j]
        #print(jobj)
        Cm[0]=Cm[0]+pp[jobj][0]
        Cjm[jobj][0]=Cm[0]
        #print('Cm[0]=',Cm[0])
        for i in range(1,m):
            Cm[i]=max(Cm[i-1],Cm[i])+pp[jobj][i]
            Cjm[jobj][i]=Cm[i]
            #print('Cm[',i,']=',Cm[i])
        #print('fin de Jj=',Cm[m-1])
    #print(Cjm)
    Cmax = Cm[m-1]
    for j in range(0,nbjobs): fake_dd[j]=Cmax-dd[j]
    #=======================================================
    # Calcul des dates de depart au plus tot des tournees
    #=======================================================
    nb_tournees = len(sol[1])
    #print('nb_tournees=',nb_tournees)
    depart_plus_tot = [0]*nb_tournees
    for u in range(0,nb_tournees):
        depart_plus_tot[u] = -1
        #print('tournee ',u,':')
        for job in sol[1][u]:
            #print('\t visite job ,',job)
            depart_plus_tot[u]=max(Cjm[job][m-1],depart_plus_tot[u])
        #print('depart au plus tot de ',u,'=',depart_plus_tot[u])
    #=======================================================
    # Calcul des dates de livraison et du retard total
    #=======================================================
    Dj=[0]*nbjobs
    Tj=[0]*nbjobs
    TotalTj = 0
    num_tournee = 0
    date_depart = depart_plus_tot[num_tournee]
    date_retour = 0
    depot = nbjobs
    for tournee in sol[1]:
        ville_depart = depot
        #print('tournee ',tournee,' de ',len(tournee),' depots')
        date_depart = max(date_retour,depart_plus_tot[num_tournee])
        for dest in tournee:
            #print('destination ',dest)
            date_arrivee = date_depart + tt[ville_depart][dest]
            Dj[dest]=date_arrivee
            if not div:
                Tj[dest]=max(0,Dj[dest]-dd[dest])
            else:
                #Tj[dest]=max(0,Dj[dest]-fake_dd[dest])
                Tj[dest]=Dj[dest]
            TotalTj = TotalTj + Tj[dest]
            #print('date arrivee =',date_arrivee, 'retard=',Tj[dest])
            date_depart = date_arrivee
            ville_depart = dest
        date_retour = date_arrivee + tt[dest][depot]
        num_tournee = num_tournee + 1
        #print ('date_retour =',date_retour)
    #print('\t TotalTj=',TotalTj)
    return(TotalTj)



# ===================================================
# Construction d'une solution initiale
# ===================================================
def SequenceInit():
    global sol_init
    global best_sol
    fake_dd = copy.deepcopy(dd)
    for j in range(0,nbjobs):
        index_the_job = fake_dd.index(min(fake_dd))
        #print(index_the_job)
        sol_init[0].append(index_the_job)
        fake_dd[index_the_job] = INFINI
    #print(sol_init[0])
    nbjobs_par_batch = 1
    val_best_sol = INFINI
    while (nbjobs_par_batch <= nbjobs/2):
        sol_batch = [[],[]]
        sol_batch[0] = copy.deepcopy(sol_init[0])
        j=0
        while (j<= nbjobs-1):
            cpt = 0
            le_batch = []
            while (cpt <= nbjobs_par_batch-1) and (j <= nbjobs-1):
                le_batch.append(sol_init[0][j])
                j=j+1
                cpt=cpt+1
            sol_batch[1].append(le_batch)
        valeur_sol_batch = evalue(sol_batch,False)
        #print(sol_batch,':',valeur_sol_batch)
        if (valeur_sol_batch < val_best_sol):
            val_best_sol = valeur_sol_batch
            best_sol = copy.deepcopy(sol_batch)
        nbjobs_par_batch = nbjobs_par_batch+1
    print('val_best_sol=',val_best_sol,':',best_sol)
    #=======================================================
    # petit 2-OPT
    #=======================================================
    if (FLAG_2OPT):
        k=0
        while k <= nbjobs-2:
            #print('tester ',best_sol[0][k],' et ',best_sol[0][k+1])
            sol_cour = copy.deepcopy(best_sol)
            val_voisin = swap_both(k,k+1,sol_cour,False)
            if(val_voisin < val_best_sol):
                best_sol = copy.deepcopy(sol_cour)
                val_best_sol = val_voisin
            k=k+1
        print('val_best_sol 2 =',val_best_sol,':',best_sol)


time_start = time.clock()
cpu=0

# ===================================================
# Calcul de la sequence initiale
# ===================================================

sol_init=[[],[]]
best_sol = [[],[]]
SequenceInit()
sol_init = copy.deepcopy(best_sol)
Best_val = evalue(sol_init,False)
#print('sol_init=',sol_init,Best_val)

##TIME_LIMIT = 0

# ===================================================
# Tabou
# ===================================================

Best_sol = copy.deepcopy(sol_init)
sol_cour = copy.deepcopy(sol_init)
Best_vois = [[],[]]

# ===================================================
# Tabou
# ===================================================
nb_ite = 0
nb_ite_sans_amel = 0
diversification = False

while (cpu < TIME_LIMIT) and (nb_ite <= NB_ITE_MAX):
    amelioration = False
    val_best_vois = INFINI
    stop_ebsr_both = False
    stop_ebsr_seq = False
    stop_ebsr_batch = False
    stop_efsr = False

    # voisinage = swap_both
    if(FLAG_SWAP_BOTH):
        for i in range(0,nbjobs-1):
            for j in range(i+1,nbjobs):
                if j-i <= DELTA:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = swap_both(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if (val_voisin < val_best_vois) and PasTabou('s',sol_cour[0][i],sol_cour[0][j]) :
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='s'
                        typeIndex_i,typeIndex_j=sol_cour[0][i],sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE): save_sol_cour = copy.deepcopy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)

    # voisinage = swap_seq
    if(FLAG_SWAP_SEQ):
        for i in range(0,nbjobs-1):
            for j in range(i+1,nbjobs):
                if j-i <= DELTA:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = swap_seq(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if (val_voisin < val_best_vois) and PasTabou('s',sol_cour[0][i],sol_cour[0][j]) :
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='s'
                        typeIndex_i,typeIndex_j=sol_cour[0][i],sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE): save_sol_cour = copy.deepcopy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)

    # voisinage = swap_batch
    if(FLAG_SWAP_BATCH):
        for i in range(0,nbjobs-1):
            for j in range(i+1,nbjobs):
                if j-i <= DELTA:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = swap_batch(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if (val_voisin < val_best_vois) and PasTabou('s',sol_cour[0][i],sol_cour[0][j]) :
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='t'
                        typeIndex_i,typeIndex_j=sol_cour[0][i],sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE): save_sol_cour = copy.deepcopy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)

    if (FLAG_EBSR_BOTH):
        i=0
        while i<=nbjobs-2 and not stop_ebsr_both:
        #for i in range(0,nbjobs-1):
            j=i+1
            while j<=nbjobs-1 and not stop_ebsr_both:
            #for j in range(i+1,nbjobs):
                if j-i <= DELTA:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = ebsr_both(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if(val_voisin < val_best_vois) and PasTabou('b',sol_cour[0][i],sol_cour[0][j]):
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='b'
                        typeIndex_i,typeIndex_j=sol_cour[0][i],sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE):
                            stop_ebsr_both = True
                            save_sol_cour = copy.deepcopy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)
                j=j+1
            i=i+1

    if (FLAG_EBSR_SEQ):
        i=0
        while (i<=nbjobs-2) and not stop_ebsr_seq:
        #for i in range(0,nbjobs-1):
            j=i+1
            while j<=nbjobs-1 and not stop_ebsr_seq:
            #for j in range(i+1,nbjobs):
                if j-i <= DELTA_EBFSR_SOLO:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = ebsr_seq(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if(val_voisin < val_best_vois) and PasTabou('b',sol_cour[0][i],sol_cour[0][j]):
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='b'
                        typeIndex_i,typeIndex_j=sol_cour[0][i],sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE):
                            stop_ebsr_seq = True
                            save_sol_cour = copy.deepcopy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)
                j=j+1
            i=i+1

    if (FLAG_EBSR_BATCH):
        i=0
        while i <= nbjobs-2 and not stop_ebsr_batch:
        #for i in range(0,nbjobs-1):
            j=i+1
            while j<= nbjobs-1 and not stop_ebsr_batch:
            #for j in range(i+1,nbjobs):
                if j-i <= DELTA_EBFSR_SOLO:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = ebsr_batch(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if(val_voisin < val_best_vois) and PasTabou('b',sol_cour[0][i],sol_cour[0][j]):
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='c'
                        typeIndex_i,typeIndex_j=sol_cour[0][i],sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE):
                            stop_ebsr_batch = True
                            save_sol_cour = copy.deepcopy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)
                j=j+1
            i=i+1


    if (FLAG_EFSR_BOTH):
        for i in range(0,nbjobs-1):
            for j in range(i+1,nbjobs):
                if j-i <= DELTA:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = efsr_both(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if(val_voisin < val_best_vois) and PasTabou('f',sol_cour[0][i],sol_cour[0][j]):
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='f'
                        typeIndex_i=sol_cour[0][i]
                        typeIndex_j=sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE): save_sol_cour = copy.copy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)

    if (FLAG_EFSR_SEQ):
        for i in range(0,nbjobs-1):
            for j in range(i+1,nbjobs):
                if j-i <= DELTA_EBFSR_SOLO:
                    #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = efsr_seq(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if(val_voisin < val_best_vois) and PasTabou('f',sol_cour[0][i],sol_cour[0][j]):
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='f'
                        typeIndex_i=sol_cour[0][i]
                        typeIndex_j=sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE): save_sol_cour = copy.copy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)


    if (FLAG_EFSR_BATCH):
        for i in range(0,nbjobs-1):
            for j in range(i+1,nbjobs):
                if j-i <= DELTA_EBFSR_SOLO:
                #print('solution courante:',sol_cour)
                    save_sol_cour = copy.deepcopy(sol_cour)
                    val_voisin = efsr_batch(i,j,sol_cour,diversification)
                    # si on est le meilleur voisin
                    if(val_voisin < val_best_vois) and PasTabou('f',sol_cour[0][i],sol_cour[0][j]):
                        val_best_vois = val_voisin
                        Best_vois = copy.deepcopy(sol_cour)
                        #print('Best_vois=',Best_vois)
                        typeBest_vois='f'
                        typeIndex_i=sol_cour[0][i]
                        typeIndex_j=sol_cour[0][j]
                        if (FLAG_FIRST_IMPROVE): save_sol_cour = copy.copy(Best_vois)
                    sol_cour = copy.deepcopy(save_sol_cour)

    #on choisit le meilleur voisin
    if(val_best_vois != INFINI):
        sol_cour = copy.copy(Best_vois)
        print(val_best_vois)
        InsereTabou(typeBest_vois,typeIndex_i,typeIndex_j)

    # si on est la meilleure solution
    if(val_best_vois < Best_val) and not diversification:
        Best_val = val_best_vois
        Best_sol = copy.deepcopy(sol_cour)
        print ('\t',Best_val)
        amelioration = True
        nb_ite_sans_amel = 0

    if(diversification):
        diversification = False
        ##print('*** fin diversification ***')

    if not amelioration:
        nb_ite_sans_amel = nb_ite_sans_amel+1

    if(nb_ite_sans_amel >= NB_ITE_SANS_AMEL_MAX):
        nb_ite_sans_amel = 0
        if (AVEC_DIVERSIFICATION):
            diversification = True
            ListeTabou = []
            print('*** on diversifie ***')

    time_end = time.clock()
    cpu = time_end-time_start
    nb_ite = nb_ite + 1


print('temps de calcul =',cpu,'(',nb_ite,') ite')

print('**************************')
print(Best_sol,Best_val)
print('**************************')

# ===================================================
# Ecriture des resultats
# ===================================================
mesresultats = open("output.txt","w")

# ecriture de la valeur
ch_Best_val = str(Best_val)
for i in range(0,len(ch_Best_val)):
    mesresultats.write(ch_Best_val[i])
mesresultats.write('\t')

# ecriture du cpu
NB_CAR_CPU = 8
if cpu>0:
    ch_cpu = str(cpu)
    for i in range(0,NB_CAR_CPU):
        mesresultats.write(ch_cpu[i])
else: mesresultats.write('0')
mesresultats.write('\t')

# ecriture de la solution
ch_Best_sol = str(Best_sol)
for i in range(0,len(ch_Best_sol)):
    mesresultats.write(ch_Best_sol[i])
mesresultats.write('\n')

mesresultats.close()

