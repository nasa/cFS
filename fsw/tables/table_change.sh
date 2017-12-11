## This is a test script to load and activate to_config_2.tbl to verify table changes.
## Notes
## 1: ValidateChecksum checks in ci_custom.c must not be performed as in UDP example.
##    Since cmdUtil does not compute a checksum for commands sent.
## 2: Ensure that your criticalMsgIds array does not include message ID that
##    Are not included in the to_config_2.tbl, otherwise table validation will 
##    fail. 


# Load to_config_2.tbl
echo "Step 1: Load the to_config_2.tbl through CFE TBL."
cmdUtil --port=5010 --endian=LE --pktid=0x1804 --cmdcode=2 --string="64:/cf/apps/to_config_2.tbl"
sleep 1
# Verify the new inactive table
echo "Step 2: Verify the new table through CFE_TBL."
cmdUtil --port=5010 --endian=LE --pktid=0x1804 --cmdcode=4 --half=0 --string="40:TO.to_config"
sleep 1
# Activate the new table (CFE TBL will call TO_ManageTable
echo "Step 3: Activate the new table through CFE_TBL."
cmdUtil --port=5010 --endian=LE --pktid=0x1804 --cmdcode=5 --string="40:TO.to_config"
sleep 1
