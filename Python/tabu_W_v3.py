##===========
## Auteur : J-C. Billaut - Polytech Tours - U. de Tours
## LI EA 6300 - ROOT ERL CNRS 6305
##===========
## Juillet 2017
##
## Ce programme calcule une solution au probleme integre ordo-routing
## avec un atelier de type flowshop pour la production et un seul
## vehicule pour la distribution. Le critere est la somme des retards.
##
## L'algorithme est un algorithme Tabou pour lequel le codage est un
## codage uniquement des batchs. Pour chaque batch, on calcule un
## ordonnancement des jobs et un routage, les meilleurs possibles.
##

# ########################################################
# Importation de packages
# ########################################################

# from tkinter import *
import os
import math
import time  # pour avoir le temsp de calcul cpu
##from numpy import *
import copy
import sys

# ########################################################
# Lecture du fichier
# ########################################################

"""
m = nombre de machines
nbjobs = nombres de taches
pp = temps machine
dd = dates dues
tt = temps de trajet
"""

if len(sys.argv) == 2:
	instanceName = sys.argv[1]
else:
	instanceName = "../Inputs/I_1_5_20_2.txt"

if not os.path.exists("log"):
	os.mkdir("log");
mesdonnees = open(instanceName, "r")
instanceName = instanceName[instanceName.rfind("/") + 1:]
ligne1 = mesdonnees.readline()
indice = 0
j = 0
while j <= len(ligne1) and ligne1[j] != '\n':
	val = ''
	while (ligne1[j] == ' ') and j <= len(ligne1) and ligne1[j] != '\n':
		j = j + 1
	indice = indice + 1
	while (ligne1[j] != ' ') and j <= len(ligne1) and ligne1[j] != '\n':
		val = val + ligne1[j]
		j = j + 1
	# print('val=',val)
	if indice == 1:
		m = int(val)
	elif indice == 2:
		nbjobs = int(val)
# print('nbjobs=',nbjobs,' m=',m)

pp = [[] for i in range(nbjobs)]
for i in range(0, m):
	ligne = mesdonnees.readline()
	laligne = ligne.split('\t')
	# print(laligne)
	if laligne != '\n':
		for j in range(0, nbjobs):
			val = int(laligne[j])
			# print('val=',val)
			pp[j].insert(i, val)
# print('pp=',pp)

dd = []
ligne = mesdonnees.readline()
laligne = ligne.split('\t')
for j in range(0, nbjobs):
	val = int(laligne[j])
	# print('val=',val)
	dd.append(val)
# print('dd=',dd)

tt = [[] for i in range(nbjobs + 1)]
for i in range(0, nbjobs + 1):
	ligne = mesdonnees.readline()
	laligne = ligne.split('\t')
	# print(laligne)
	if laligne != '\n':
		for j in range(0, nbjobs + 1):
			val = int(laligne[j])
			# print('val=',val)
			tt[j].insert(i, val)
# print('tt=',tt)

mesdonnees.close()

##nbjobs = 3
##nbmachines = 2
##m=2
##pp=[[1,100],[100,1],[1,100]]
##dd=[203,152,203]
##tt=[[0,51,0,1],[51,0,51,50],[0,51,0,1],[1,50,1,0]]

# ===================================================
# Constantes
# ===================================================

nbjobs = len(pp)
INFINI = 9999999
ListeTabou = []
TailleTabou = 5
TIME_LIMIT = nbjobs * m / 4

if nbjobs == 100:
	DELTA = 8
elif nbjobs == 50:
	DELTA = 10
else:
	DELTA = nbjobs / 4

DELTA_NB_BATCHS = 4
NB_ITE_MAX = 2000
##NB_ITE_SANS_AMEL_MAX = 20 #NB_ITE_MAX /10
if nbjobs == 100:
	NB_ITE_SANS_AMEL_MAX = 5
elif nbjobs == 50:
	NB_ITE_SANS_AMEL_MAX = 10
else:
	NB_ITE_SANS_AMEL_MAX = 15

AVEC_DIVERSIFICATION = 1

FLAG_EBSR = 1
FLAG_EFSR = 1
FLAG_SWAP = 1
FLAG_2OPT = 0
FLAG_FIRST_IMPROVE = 0
TABOU_LOGIQUE = 1  # 1 = je swappe(i,j) donc (j,i) est tabou


def flagsFingerprint():
	i = 0
	if FLAG_SWAP: i += 1
	if FLAG_EBSR: i += 10
	if FLAG_EFSR: i += 100
	if AVEC_DIVERSIFICATION: i += 1000
	if FLAG_FIRST_IMPROVE: i += 10000
	return i


def InsereTabou(voisinage, indi, indj):
	ElemTabou = [voisinage, indi, indj]
	if len(ListeTabou) == TailleTabou:
		del (ListeTabou[0])
	ListeTabou.append(ElemTabou)


# print('ListeTabou :',ListeTabou)


def PasTabou(voisinage, indi, indj):
	ElemTabou = [voisinage, indi, indj]
	pasTabou = True
	if (ElemTabou in ListeTabou):
		# print('Element ',ElemTabou,' dans la liste Tabou')
		pasTabou = False
	return (pasTabou)


def swap2(b_i, i_i, b_j, i_j, sol, div):
	# print('avant swap:',b_i,i_i,b_j,i_j,sol)
	sol[b_i][i_i], sol[b_j][i_j] = sol[b_j][i_j], sol[b_i][i_i]
	# print('apres swap:',sol)
	valeur = evalue(sol, div, False)
	# print(valeur)
	return (valeur)


def ebsr2(b_i, b_j, i_j, sol, div):
	# print('avant ebsr:',b_i,b_j,i_j,sol)
	if (len(sol[b_j]) > 1):
		# print('plus d un job dans le batch')
		sol[b_i].append(sol[b_j][i_j])
		del (sol[b_j][i_j])
	else:
		# print('un seul job dans le batch')
		sol[b_i].append(sol[b_j][i_j])
		del (sol[b_j])
	valeur = evalue(sol, div, False)
	# print('apres ebsr:',sol,i,j)
	return (valeur)


def efsr2(b_i, i_i, b_j, sol, div):
	# la sequence d'entree n'est pas modifiee par ebsr
	# print('avant efsr:',b_i,i_i,b_j,sol)
	if (len(sol[b_i]) > 1):
		# print('plus d un job dans le batch')
		sol[b_j].append(sol[b_i][i_i])
		del (sol[b_i][i_i])
	else:
		# print('un seul job dans le batch')
		sol[b_j].append(sol[b_i][i_i])
		del (sol[b_i])
	valeur = evalue(sol, div, False)
	# print('apres ebsr:',sol,i,j)
	return (valeur)


def calcule_makespan(seq):
	global Cm
	cmbis = [0] * m
	for i in range(0, m): cmbis[i] = Cm[i]
	makespan = 0
	for j in range(0, len(seq)):
		cmbis[0] = cmbis[0] + pp[seq[j]][0]
		for i in range(1, m):
			cmbis[i] = max(cmbis[i - 1], cmbis[i]) + pp[seq[j]][i]
	makespan = cmbis[m - 1]
	return (makespan)


# =====================================================
# Scheduling of a batch -- to modify
# =====================================================
def sequence_the_jobs(set_of_jobs, divers):
	global Cm
	nbjobs_here = len(set_of_jobs)
	# print('Cm=',Cm)
	cmbis = [0] * m
	la_somme = [0] * nbjobs_here
	new_sequence = copy.deepcopy(set_of_jobs)
	# print(set_of_jobs)

	# S il y a plus de 3 jobs on calcule
	if (nbjobs_here > 3):
		# print('more than 3 jobs -- on fait NEH sans les prendre dans un ordre particulier')
		sequence_test = [-1] * nbjobs_here
		job1 = set_of_jobs[0]
		job2 = set_of_jobs[1]
		sequence_test[0] = job1
		sequence_test[1] = job2
		if calcule_makespan(sequence_test) > calcule_makespan([job2, job1]):
			sequence_test[0], sequence_test[1] = sequence_test[1], sequence_test[0]
		##        if not divers \
		##           and (calcule_makespan(sequence_test) > calcule_makespan([job2,job1])):
		##            sequence_test[0],sequence_test[1]=sequence_test[1],sequence_test[0]
		##        if divers \
		##           and (calcule_makespan(sequence_test) < calcule_makespan([job2,job1])):
		##            sequence_test[0],sequence_test[1]=sequence_test[1],sequence_test[0]

		# print('sequence_test=',sequence_test)
		for j in range(2, nbjobs_here):
			lejob = set_of_jobs[j]
			# print('lejob=',lejob,'sequence_test=',sequence_test)
			best_makespan = INFINI
			##            if not divers: best_makespan = INFINI
			##            else: best_makespan = 0
			for pos_possible in range(0, j + 1):
				# print('position possible :',pos_possible)
				sequence_essai = sequence_test[0:pos_possible] + [lejob] + sequence_test[pos_possible:j]
				this_makespan = calcule_makespan(sequence_essai)
				if this_makespan < best_makespan:
					save_sequence_essai = copy.deepcopy(sequence_essai)
					best_makespan = this_makespan
			##                if not divers and (this_makespan < best_makespan):
			##                    save_sequence_essai = copy.deepcopy(sequence_essai)
			##                    best_makespan = this_makespan
			##                    #print('meilleure =',save_sequence_essai,this_makespan)
			##                elif divers and (this_makespan > best_makespan):
			##                    save_sequence_essai = copy.deepcopy(sequence_essai)
			##                    best_makespan = this_makespan
			# print(sequence_essai)
			sequence_test = copy.deepcopy(save_sequence_essai)

	# S il y a 3 jobs, on enumere
	elif (nbjobs_here == 3):
		job0 = set_of_jobs[0]
		job1 = set_of_jobs[1]
		job2 = set_of_jobs[2]
		makespan012 = calcule_makespan([job0, job1, job2])
		makespan021 = calcule_makespan([job0, job2, job1])
		makespan102 = calcule_makespan([job1, job0, job2])
		makespan120 = calcule_makespan([job1, job2, job0])
		makespan201 = calcule_makespan([job2, job0, job1])
		makespan210 = calcule_makespan([job2, job1, job0])
		makespan_min = min(makespan012, makespan021, makespan102, makespan120, \
		                   makespan201, makespan210)
		##        if divers:
		##            # we take the worse solution
		##            makespan_min = max(makespan012,makespan021,makespan102,makespan120,\
		##            makespan201,makespan210)

		if (makespan021 == makespan_min):
			new_sequence[1], new_sequence[2] = new_sequence[2], new_sequence[1]
		elif (makespan102 == makespan_min):
			new_sequence[0], new_sequence[1] = new_sequence[1], new_sequence[0]
		elif (makespan120 == makespan_min):
			tmp = new_sequence[0]
			new_sequence[0] = new_sequence[1]
			new_sequence[1] = new_sequence[2]
			new_sequence[2] = tmp
		elif (makespan201 == makespan_min):
			tmp = new_sequence[0]
			new_sequence[0] = new_sequence[2]
			new_sequence[2] = new_sequence[1]
			new_sequence[1] = tmp
		elif (makespan210 == makespan_min):
			new_sequence[0], new_sequence[2] = new_sequence[2], new_sequence[0]

	# s il y a 2 jobs on enumere
	elif (nbjobs_here == 2):
		job1 = set_of_jobs[0]
		job2 = set_of_jobs[1]
		makespan12 = calcule_makespan([job1, job2])
		makespan21 = calcule_makespan([job2, job1])
		# print(makespan12,makespan21)
		if makespan21 < makespan12:
			new_sequence[0], new_sequence[1] = new_sequence[1], new_sequence[0]
	##        if not divers and (makespan21<makespan12):
	##            new_sequence[0],new_sequence[1]=new_sequence[1],new_sequence[0]
	##        elif divers and (makespan12<makespan21):
	##            new_sequence[0],new_sequence[1]=new_sequence[1],new_sequence[0]

	# print('new_sequence=',new_sequence)
	return (new_sequence)


# =====================================================
# Routing of a batch -- to modify
# =====================================================
def route_the_jobs(set_of_jobs, date, divers):
	# prenons EDD
	nbjobs_here = len(set_of_jobs)
	new_sequence = copy.deepcopy(set_of_jobs)

	# S il y a plus de 3 jobs on calcule
	if (nbjobs_here > 3):
		# print('more than 3 jobs -- on fait EDD')
		fake_edd = [-1] * nbjobs_here
		for j in range(0, nbjobs_here):
			fake_edd[j] = dd[set_of_jobs[j]]
		# print(fake_edd)
		ind_seq = 0
		for j in range(0, nbjobs_here):
			the_job = fake_edd.index(min(fake_edd))
			# print('the_job=',the_job)
			if diversification:
				the_job = fake_edd.index(max(fake_edd))
			new_sequence[ind_seq] = set_of_jobs[the_job]
			fake_edd[the_job] = INFINI
			ind_seq = ind_seq + 1

	# S il y a 3 jobs, on enumere
	elif (nbjobs_here == 3):
		job0 = set_of_jobs[0]
		job1 = set_of_jobs[1]
		job2 = set_of_jobs[2]
		tardiness012 = compute_tardiness([job0, job1, job2], date, False, divers)
		tardiness021 = compute_tardiness([job0, job2, job1], date, False, divers)
		tardiness102 = compute_tardiness([job1, job0, job2], date, False, divers)
		tardiness120 = compute_tardiness([job1, job2, job0], date, False, divers)
		tardiness201 = compute_tardiness([job2, job0, job1], date, False, divers)
		tardiness210 = compute_tardiness([job2, job1, job0], date, False, divers)
		tardiness_min = min(tardiness012, tardiness021, tardiness102, tardiness120, \
		                    tardiness201, tardiness210)
		##        if divers:
		##            # we consider the worse solution
		##            tardiness_min = max(tardiness012,tardiness021,tardiness102,tardiness120,\
		##            tardiness201,tardiness210)
		if (tardiness021 == tardiness_min):
			new_sequence[1], new_sequence[2] = new_sequence[2], new_sequence[1]
		elif (tardiness102 == tardiness_min):
			new_sequence[0], new_sequence[1] = new_sequence[1], new_sequence[0]
		elif (tardiness120 == tardiness_min):
			tmp = new_sequence[0]
			new_sequence[0] = new_sequence[1]
			new_sequence[1] = new_sequence[2]
			new_sequence[2] = tmp
		elif (tardiness201 == tardiness_min):
			tmp = new_sequence[0]
			new_sequence[0] = new_sequence[2]
			new_sequence[2] = new_sequence[1]
			new_sequence[1] = tmp
		elif (tardiness210 == tardiness_min):
			new_sequence[0], new_sequence[2] = new_sequence[2], new_sequence[0]

	# s il y a 2 jobs on enumere
	elif (nbjobs_here == 2):
		job1 = set_of_jobs[0]
		job2 = set_of_jobs[1]
		tardiness12 = compute_tardiness([job1, job2], date, False, divers)
		tardiness21 = compute_tardiness([job2, job1], date, False, divers)
		# print(tardiness12,tardiness21)
		if tardiness21 < tardiness12:
			new_sequence[0], new_sequence[1] = new_sequence[1], new_sequence[0]
	##        if not divers and (tardiness21<tardiness12):
	##            new_sequence[0],new_sequence[1]=new_sequence[1],new_sequence[0]
	##        elif divers and (tardiness12<tardiness21):
	##            new_sequence[0],new_sequence[1]=new_sequence[1],new_sequence[0]

	# print('new_sequence_routing=',new_sequence)
	return (new_sequence)


def update_Cjm_Cm(sequence):
	global Cjm
	global Cm
	for j in range(0, len(sequence)):
		jj = sequence[j]
		# print('\t jj=',jj)
		Cm[0] = Cm[0] + pp[jj][0]
		Cjm[jj][0] = Cm[0]
		# print('\t Cjm[',jj,'][0]=',Cjm[jj][0])
		for i in range(1, m):
			Cm[i] = max(Cm[i - 1], Cm[i]) + pp[jj][i]
			Cjm[jj][i] = Cm[i]
		# print('\t Cjm[',jj,'][',i,']=',Cjm[jj][i])


# print('\t Cjm=',Cjm)


def compute_tardiness(sequence, date, pour_de_vrai, div):
	global vehicle_back
	# print(sequence,'at this time:',date)
	# print('vehicle back=',vehicle_back)
	save_vehicle_back = vehicle_back
	depart_site = nbjobs
	tardiness = 0
	for job in sequence:
		arrival_site = job
		# print('travel from site',depart_site,'to',arrival_site,tt[depart_site][arrival_site])
		this_time = tt[depart_site][arrival_site]
		arrival_time = date + this_time
		# print('arrival_time=',arrival_time)
		tardiness = tardiness + max(0, arrival_time - dd[job])
		if (div): tardiness = tardiness + max(0, dd[job] - arrival_time)
		##if (div): tardiness = tardiness + arrival_time
		# print('total tardiness = ',tardiness)
		depart_site = job
		date = arrival_time
	vehicle_back = arrival_time + tt[depart_site][nbjobs]
	if not pour_de_vrai: vehicle_back = save_vehicle_back
	# print('vehicle_back=',vehicle_back)
	return (tardiness)


# ===================================================
# Evaluation d'une solution
# sol : to be evaluated
# diversification : if False then normal evaluation, opposite if True
# final : if True, final evaluation, return the sequences
# ===================================================
def evalue(sol, diversification, Final):
	global Cjm
	global Cm
	global vehicle_back
	TotalTj = 0
	vehicle_back = 0
	nb_batches = len(sol)
	Cm = [0] * m
	Cjm = [[0 for i in range(m)] for i in range(nbjobs)]
	# print('nbbatches=',nb_batches)
	# for each batch

	if Final:
		logSolutionEnd = open("./log/solution_" + instanceName + "_" + str(flagsFingerprint()) + ".txt", "w")
		logSolutionEnd.write("P\n")
	for one_batch in sol:
		nb_jobs_batch = len(one_batch)
		the_jobs = one_batch
		# print(nb_jobs_batch,the_jobs)
		seq_schedule = sequence_the_jobs(the_jobs, diversification)
		update_Cjm_Cm(seq_schedule)
		if Final:
			print('sched=', seq_schedule)
			logSolutionEnd.write('sched= ' + str(seq_schedule) + "\n")
		min_departure_batch = Cjm[seq_schedule[nb_jobs_batch - 1]][m - 1]
		# print('date de fin dernier job=',Cjm[seq_schedule[nb_jobs_batch-1]][m-1])
		departure_batch = max(min_departure_batch, vehicle_back)
		# print('departure_batch=',departure_batch)
		seq_routing = route_the_jobs(the_jobs, departure_batch, diversification)
		if Final:
			print('route=', seq_routing)
			logSolutionEnd.write('route= ' + str(seq_routing) + "\n")
		tardiness = compute_tardiness(seq_routing, departure_batch, True, diversification)
		TotalTj = TotalTj + tardiness
	if Final:
		logSolutionEnd.write(str(TotalTj) + "\n")
		logSolutionEnd.close()
	# print(tardiness,TotalTj)
	return (TotalTj)


# ===================================================
# Construction d'une solution initiale
# ===================================================
def SequenceInit():
	global sol_init
	global best_sol
	fake_dd = copy.deepcopy(dd)
	for j in range(0, nbjobs):
		index_the_job = fake_dd.index(min(fake_dd))
		sol_init.append(index_the_job)
		fake_dd[index_the_job] = INFINI
	nbjobs_par_batch = 1
	val_best_sol = INFINI
	while (nbjobs_par_batch <= nbjobs / 2):
		sol_batch = []
		j = 0
		while (j <= nbjobs - 1):
			cpt = 0
			le_batch = []
			while (cpt <= nbjobs_par_batch - 1) and (j <= nbjobs - 1):
				le_batch.append(sol_init[j])
				j = j + 1
				cpt = cpt + 1
			sol_batch.append(le_batch)
		valeur_sol_batch = evalue(sol_batch, False, False)
		# print(sol_batch,':',valeur_sol_batch)
		if (valeur_sol_batch < val_best_sol):
			val_best_sol = valeur_sol_batch
			best_sol = copy.deepcopy(sol_batch)
		nbjobs_par_batch = nbjobs_par_batch + 1
	print('val_best_sol 1 =', val_best_sol, ':', best_sol)
	# =======================================================
	# petit 2-OPT
	# =======================================================
	if FLAG_2OPT:
		k = 0
		while k <= len(best_sol) - 2:
			# print('tester ',best_sol[k],' et ',best_sol[k+1])
			l1 = 0
			while (l1 <= len(best_sol[k]) - 1):
				jj1 = best_sol[k][l1]
				l2 = 0
				while (l2 <= len(best_sol[k + 1]) - 1):
					jj2 = best_sol[k + 1][l2]
					# print('\t tester',jj1,jj2)
					sol_test = copy.deepcopy(best_sol)
					sol_test[k][l1], sol_test[k + 1][l2] = sol_test[k + 1][l2], sol_test[k][l1]
					sumTj_test = evalue(sol_test, False, False)
					# print('\t',sol_test,sumTj_test)
					if (sumTj_test < val_best_sol):
						val_best_sol = sumTj_test
						best_sol = copy.deepcopy(sol_test)
						sol_test = copy.deepcopy(best_sol)
						jj1 = best_sol[k][l1]
						jj2 = best_sol[k + 1][l2]
					# print('**')
					l2 = l2 + 1
				l1 = l1 + 1
			k = k + 1
		print('val_best_sol 2 =', val_best_sol, ':', best_sol)


# ===================================================
# Initialisations
# ===================================================
time_start = time.clock()
cpu = 0
sol_init = []
best_sol = []
diversification = False
SequenceInit()
sol_init = copy.deepcopy(best_sol)

Best_val = evalue(sol_init, False, False)
# print('sol_init=',sol_init,Best_val,'(',cpu,')')

##TIME_LIMIT = 0

# ===================================================
# Tabou
# ===================================================

Best_sol = copy.deepcopy(sol_init)
sol_cour = copy.deepcopy(sol_init)
Best_vois = []
nb_ite = 0
nb_ite_sans_amel = 0
diversification = False

scoreLog = open("./log/log_" + instanceName + "_" + str(flagsFingerprint()) + ".csv", "w")
scoreLogCompact = open("./log/scoreLogPythonCompact_" + instanceName + "_" + str(flagsFingerprint()) + ".csv", "w")
scoreLogFull = open("./log/scoreLogPythonFull_" + instanceName + "_" + str(flagsFingerprint()) + ".csv", "w")
scoreLog.write("P_BestEver,P_BestIter\n")
scoreLogCompact.write("P_BestIter,P_BestEver\n")
scoreLogFull.write("P_Iter,P_BestEver\n")
scoreLogFull.write(str(0))
scoreLogFull.write(",")
scoreLogFull.write(str(sol_cour))
scoreLogFull.write("\n")
# BOUCLE GENERALE
while (cpu < TIME_LIMIT) and (nb_ite <= NB_ITE_MAX):
	amelioration = False
	val_best_vois = INFINI
	stop_ebsr = False  # utilise pour FIRT_IMROVE
	stop_efsr = False
	# print('SOLUTION COURANTE :',sol_cour)

	# voisinage = swap
	if (FLAG_SWAP):
		nb_batches = len(sol_cour)
		# print(sol_cour)
		batch_i = 0
		# for batch_i in range(0,nb_batches-1):
		while batch_i <= nb_batches - 2:
			index_i = 0
			while index_i <= len(sol_cour[batch_i]) - 1:
				# for job_i in sol_cour[batch_i]:
				job_i = sol_cour[batch_i][index_i]
				batch_j = batch_i + 1
				while batch_j <= nb_batches - 1:
					# for batch_j in range(batch_i+1,nb_batches):
					index_j = 0
					while index_j <= len(sol_cour[batch_j]) - 1:
						# for job_j in sol_cour[batch_j]:
						job_j = sol_cour[batch_j][index_j]
						if index_j - index_i <= DELTA:
							# print('SWAP job_i=',job_i,'job_j=',job_j)
							save_sol_cour = copy.deepcopy(sol_cour)
							# print('SWAP sol_cour avant swap',sol_cour)
							val_voisin = swap2(batch_i, index_i, batch_j, index_j, sol_cour, diversification)
							# print('SWAP sol_cour apres swap',sol_cour,val_voisin)
							# si on est le meilleur voisin
							if (val_voisin < val_best_vois) and PasTabou('s', job_i, job_j):
								val_best_vois = val_voisin
								Best_vois = copy.deepcopy(sol_cour)

								# print('SWAP Best_vois=',Best_vois,'(',val_best_vois,')')
								typeBest_vois = 's'
								if TABOU_LOGIQUE:
									typeIndex_i, typeIndex_j = job_j, job_i
								else:
									typeIndex_i, typeIndex_j = job_i, job_j
								if (FLAG_FIRST_IMPROVE): save_sol_cour = copy.deepcopy(sol_cour)
							sol_cour = copy.deepcopy(save_sol_cour)
							index_j = index_j + 1
					batch_j = batch_j + 1
				index_i = index_i + 1
			batch_i = batch_i + 1

	if (FLAG_EBSR):
		nb_batches = len(sol_cour)
		# print(sol_cour)
		# batch_i va recevoir job_j
		# indice_du_batch_i = 0
		batch_i = 0
		while (batch_i <= nb_batches - 2) and not stop_ebsr:
			##for batch_i in range(0,nb_batches-1):
			batch_j = batch_i + 1
			# indice_du_batch_j = indice_du_batch_i + len(sol_cour[batch_i])
			while batch_j <= nb_batches - 1 and batch_j <= batch_i + DELTA_NB_BATCHS and not stop_ebsr:
				##for batch_j in range(batch_i+1,nb_batches):
				# if indice_fin_batch(batch_j,sol_cour) - indice_deb_batch(batch_i,sol_cour) <= DELTA:
				# if batch_j - batch_i <= DELTA_NB_BATCHS:
				index_j = 0
				##while index_j <= len(sol_cour[batch_j])-1 and not stop_ebsr:
				for job_j in sol_cour[batch_j]:
					##job_j = sol_cour[batch_j][index_j]
					if not stop_ebsr:
						# print('EBSR batch_i=',batch_i,'va recevoir job_j=',job_j)
						save_sol_cour = copy.deepcopy(sol_cour)
						# print('EBSR sol_cour avant ebsr',sol_cour)
						val_voisin = ebsr2(batch_i, batch_j, index_j, sol_cour, diversification)
						# print('sol_cour apres ebsr',sol_cour,val_voisin)
						# si on est le meilleur voisin
						if (val_voisin < val_best_vois) and PasTabou('b', batch_i, job_j):
							val_best_vois = val_voisin
							Best_vois = copy.deepcopy(sol_cour)
							# print('EBSR Best_vois=',Best_vois,'(',val_best_vois,')')
							typeIndex_i, typeIndex_j = batch_i, job_j
							typeBest_vois = 'b'
							if (FLAG_FIRST_IMPROVE):
								stop_ebsr = True
								save_sol_cour = copy.deepcopy(sol_cour)
						sol_cour = copy.deepcopy(save_sol_cour)
						index_j = index_j + 1
				# indice_du_batch_j = indice_du_batch_j + len(sol_cour[batch_j])
				batch_j = batch_j + 1
			# indice_du_batch_i = indice_du_batch_i + len(sol_cour[batch_i])
			batch_i = batch_i + 1

	if (FLAG_EFSR):
		nb_batches = len(sol_cour)
		# print(sol_cour)
		# batch_j va recevoir job_i
		batch_i = 0
		while (batch_i <= nb_batches - 2) and not stop_efsr:
			##for batch_i in range(0,nb_batches-1):
			index_i = 0
			for job_i in sol_cour[batch_i]:
				batch_j = batch_i + 1
				while (batch_j <= nb_batches - 1) and batch_j <= batch_i + DELTA_NB_BATCHS and not stop_efsr:
					##for batch_j in range(batch_i+1,nb_batches):
					# if batch_j - batch_i <= DELTA_NB_BATCHS:
					# print('EFSR job_i=',job_i,'va aller dans batch_j=',batch_j)
					save_sol_cour = copy.deepcopy(sol_cour)
					# print('EFSR sol_cour avant ebsr',sol_cour)
					val_voisin = efsr2(batch_i, index_i, batch_j, sol_cour, diversification)
					# print('EFSR sol_cour apres ebsr',sol_cour,val_voisin)
					# si on est le meilleur voisin
					if (val_voisin < val_best_vois) and PasTabou('f', i, j):
						val_best_vois = val_voisin
						Best_vois = copy.deepcopy(sol_cour)
						# print('EFSR Best_vois=',Best_vois,'(',val_best_vois,')')
						typeBest_vois = 'f'
						typeIndex_i, typeIndex_j = job_i, batch_j
						if (FLAG_FIRST_IMPROVE):
							stop_efsr = True
							save_sol_cour = copy.deepcopy(sol_cour)
					sol_cour = copy.deepcopy(save_sol_cour)
					# index_j = index_j+1
					batch_j = batch_j + 1
			batch_i = batch_i + 1

	# on choisit le meilleur voisin
	if (val_best_vois != INFINI):
		sol_cour = copy.copy(Best_vois)
		print(val_best_vois)
		InsereTabou(typeBest_vois, typeIndex_i, typeIndex_j)
	else:
		print("tous les voisins valent infini ???")

	# si on est la meilleure solution
	if (val_best_vois < Best_val) and not diversification:
		Best_val = val_best_vois
		Best_sol = copy.deepcopy(sol_cour)
		print('\t', Best_val)
		amelioration = True
		nb_ite_sans_amel = 0
		scoreLogCompact.write(str(nb_ite + 1))
		scoreLogCompact.write(",")
		scoreLogCompact.write(str(Best_val))
		scoreLogCompact.write("\n")

	if (diversification):
		diversification = False
	##print('*** fin diversification ***')

	if not amelioration:
		nb_ite_sans_amel = nb_ite_sans_amel + 1

	if (nb_ite_sans_amel >= NB_ITE_SANS_AMEL_MAX):
		nb_ite_sans_amel = 0
		if (AVEC_DIVERSIFICATION):
			diversification = True
			ListeTabou = []
			print('*** on diversifie ***')

	#scoreLog.write(str(Best_val))
	#scoreLog.write(";")
	#scoreLog.write(str(val_best_vois))
	#scoreLog.write("\n")
	#scoreLogFull.write(str(nb_ite + 1))
	#scoreLogFull.write(";")
	#scoreLogFull.write(str(sol_cour))
	#scoreLogFull.write("\n")
	time_end = time.clock()
	cpu = time_end - time_start
	# evalue(sol_cour, False, True)
	print("IT {}, tabu = {}".format(nb_ite, ListeTabou))
	nb_ite = nb_ite + 1
	print("-------------------------------------------")

scoreLogCompact.write(str(nb_ite + 1))
scoreLogCompact.write(",")
scoreLogCompact.write(str(Best_val))
scoreLogCompact.write("\n")
scoreLog.close()
scoreLogFull.close()
scoreLogCompact.close()
print('temps de calcul =', cpu, '(', nb_ite, ') ite')

print('**************************')
Best_val = evalue(Best_sol, False, False)
print(Best_sol, Best_val)
print('**************************')

# =======================================================
# petit 2-OPT
# =======================================================
val_best_sol = Best_val
if FLAG_2OPT:
	k = 0
	while k <= len(Best_sol) - 2:
		# print('tester ',Best_sol[k],' et ',Best_sol[k+1])
		l1 = 0
		while (l1 <= len(Best_sol[k]) - 1):
			jj1 = Best_sol[k][l1]
			l2 = 0
			while (l2 <= len(Best_sol[k + 1]) - 1):
				jj2 = Best_sol[k + 1][l2]
				# print('\t tester',jj1,jj2)
				sol_test = copy.deepcopy(Best_sol)
				sol_test[k][l1], sol_test[k + 1][l2] = sol_test[k + 1][l2], sol_test[k][l1]
				sumTj_test = evalue(sol_test, False, False)
				# print('\t',sol_test,sumTj_test)
				if (sumTj_test < val_best_sol):
					val_best_sol = sumTj_test
					Best_sol = copy.deepcopy(sol_test)
					sol_test = copy.deepcopy(Best_sol)
					jj1 = Best_sol[k][l1]
					jj2 = Best_sol[k + 1][l2]
				# print('**')
				l2 = l2 + 1
			l1 = l1 + 1
		k = k + 1

print('**************************')
Best_val = evalue(Best_sol, False, True)
print(Best_sol, Best_val)
print('**************************')

# ===================================================
# Ecriture des resultats
# ===================================================
mesresultats = open("output.txt", "w")

# ecriture de la valeur
ch_Best_val = str(Best_val)
for i in range(0, len(ch_Best_val)):
	mesresultats.write(ch_Best_val[i])
mesresultats.write('\t')

# ecriture du cpu
NB_CAR_CPU = 8
if cpu > 0:
	ch_cpu = str(cpu)
	for i in range(0, NB_CAR_CPU):
		mesresultats.write(ch_cpu[i])
else:
	mesresultats.write('0')
mesresultats.write('\t')

# ecriture de la solution
ch_Best_sol = str(Best_sol)
for i in range(0, len(ch_Best_sol)):
	mesresultats.write(ch_Best_sol[i])
mesresultats.write('\n')
mesresultats.close()
