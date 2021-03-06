#!/bin/bash
#############################################################
# Use this script in order to run faulty cache scenarios    #
#                                                           # 
# Configuation parameters:                                  #
#   - l1_size:  A list of simulating cache sizes            #
#   - l1_assoc: A list of simulation cache associativities  #
#   - l1_subblocks: Choose the block granularity            #
#           1: Block disabling (BDIS)                       #
#           2: 2 subBlocks disabling (SBDIS2)               #
#           4: 4 subblocks disabling (SBDIS4)               #
#           8: 8 subblocks disabling (SBDIS8)               #
#############################################################

## -- Simulation confifurations -- ##

threads=4            # number of available threads for parallelism
FILE="run_faulty_cache"
RESULTS_DIR=$PWD"/faultyCacheResults"
TRACES_PATH=$PWD"/polybench_traces"
BENCHMARKS="atax bicg mvt"


#########################################################
###-------------------Check Paths---------------------###
if [ -d "$RESULTS_DIR" ]; then
  echo 'Results directory found @ '$RESULTS_DIR
else
  mkdir "$RESULTS_DIR"
  echo 'Info: Results directory created @ '$RESULTS_DIR
fi

if [ -d "$TRACES_PATH" ]; then
  echo 'Trace path found @ '$TRACES_PATH
else
  echo 'ERROR: Trace path not found @ '$TRACES_PATH
  exit 1;
fi


# Memory configurations
l1_size="32 64"
l1_assoc="4 8"
l1_subblocks="1 2 4 8"

rm -f $FILE* 
echo "#!/bin/bash" > $FILE.sh 
 

let running_jobs=1

for size in $l1_size 
do
    for assoc in $l1_assoc
    do
        for subblocks in $l1_subblocks
        do  
            awk '{     if ($2=="CACHE_SIZE_1") {print $1, $2,'$size' ;}
                  else if ($2=="CACHE_WAYS")   {print $1,$2,'$assoc' ;}
                  else if ($3=="FAULTY_BEHAVIOR") {print $2,$3 ;}
                  else if ($2=="BLOCK_GRANULARITY") {print $1,$2,'$subblocks' ;}
                  else    {print $0}   }' cache_temp.h > cache.h.temp ; mv cache.h.temp cache.h 
      	    
            make clean  
            make -j$threads

            mv my_cache my_cache_"$size"_"$assoc"_"$subblocks" ;
            mv my_cache_"$size"_"$assoc"_"$subblocks" $RESULTS_DIR
            cp cache.h cache_"$size"_"$assoc"_"$subblocks".h ;
            mv cache_"$size"_"$assoc"_"$subblocks".h $RESULTS_DIR
	
            for app in $BENCHMARKS
            do
                trace=$TRACES_PATH/trace.$app

                if [ $running_jobs -eq $threads ]; then 
                    echo ' ./my_cache_'$size'_'$assoc'_'$subblocks' '$trace' >  out_'$size'_'$assoc'_'$app'_'$subblocks'   ;  ' >> "$FILE".sh
                	let running_jobs=0;
                else
            	    echo ' ./my_cache_'$size'_'$assoc'_'$subblocks' '$trace' >  out_'$size'_'$assoc'_'$app'_'$subblocks'   &  ' >> "$FILE".sh
            	    let running_jobs++;
                fi	
                echo " adding ....... "
                echo " adding ....... "
            done
        done
	done
done    

##############################################################################
# make the new.check script ready to go
chmod 777 $FILE.sh
##############################################################################
# final touch 
mv $FILE.sh $RESULTS_DIR ;
###############################################################################



