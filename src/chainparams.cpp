// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2015 The Dogecoin Core developers
// Copyright (c) 2016 The LemonCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include <stdio.h>

#include <boost/assign/list_of.hpp>

using namespace std;

#include "chainparamsseeds.h"

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
protected:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CMainParams() {
        strNetworkID = "main";

        consensus.nSubsidyHalvingInterval = 525000;
        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 60;
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fAllowLegacyBlocks = true;
        consensus.fAllowAuxPow = false;
        consensus.nHeightEffective = 0;
        consensus.fDigishieldDifficultyCalculation = true;
        consensus.nCoinbaseMaturity = 60;
        
        digishieldConsensus = consensus;
        auxpowConsensus = digishieldConsensus;
        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa8;
        pchMessageStart[1] = 0xf0;
        pchMessageStart[2] = 0xc3;
        pchMessageStart[3] = 0xc0;
        vAlertPubKey = ParseHex("04d4da7a5dae4db797d9b0644d57a5cd50e05a70f36091cd62e2fc41c98ded06340be5a43a35e185690cd9cde5d72da8f6d065b499b06f51dcfba14aad859f443a");
        nDefaultPort = 22331;
        nMinerThreads = 0;
        nPruneAfterHeight = 100000;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database.
         *
         * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1386325540, nBits=0x1e0ffff0, nNonce=99943, vtx=1)
         *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
         *   vMerkleTree: 4a5e1e
         */
        const char* pszTimestamp = "5th March 2016: Apple case exposes ongoing government rift over encryption policy";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion.SetGenesisVersion(1);
        genesis.nTime    = 1455393600;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 512785;
        
        // If genesis block hash does not match, then generate new genesis hash.
        if (false) {
        printf("Searching for %s genesis block...\n", strNetworkID.c_str());
        // This will figure out a valid hash and Nonce if you're
        // creating a different genesis block:
        arith_uint256 hashTarget = UintToArith256(consensus.powLimit);
        while(true) {
        arith_uint256 thash = UintToArith256(genesis.GetPoWHash());
        if (thash <= hashTarget)
            break;
        if ((genesis.nNonce & 0xFFF) == 0) {
        printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
        printf("NONCE WRAPPED, incrementing time\n");
        ++genesis.nTime;
        }
        }
        printf("%s genesis.nTime = %u \n", strNetworkID.c_str(), genesis.nTime);
        printf("%s genesis.nNonce = %u \n", strNetworkID.c_str(), genesis.nNonce);
        printf("%s genesis.GetPoWHash = %s\n", strNetworkID.c_str(), genesis.GetPoWHash().ToString().c_str());
        printf("%s genesis.GetHash = %s\n", strNetworkID.c_str(), genesis.GetHash().ToString().c_str());
        printf("%s genesis.hashMerkleRoot = %s\n", strNetworkID.c_str(), genesis.hashMerkleRoot.ToString().c_str());
        }
        
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0xb417b77aff17f47301ff5c6edba9c8d0751ce99e9c72525232bb15eb42c9be11"));
        assert(genesis.hashMerkleRoot == uint256S("0xcdbcb6d3b0d8ff88473f23c943e5c63265db188d7c681397238e964f8f87a44f"));

        vSeeds.push_back(CDNSSeedData("45.32.180.199", "45.32.180.199"));
        //vSeeds.push_back(CDNSSeedData("lemoncoin.net", "seed.lemoncoin.net"));
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,33);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x01)(0x9b)(0x17)(0x48).convert_to_container<std::vector<unsigned char> >(); // Lmnp
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x01)(0x9b)(0x17)(0x5d).convert_to_container<std::vector<unsigned char> >(); // Lmns

        //TODO: fix this for lemoncoin -- plddr
        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        vFixedSeeds.clear();

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of

            (      0, uint256S("0xb417b77aff17f47301ff5c6edba9c8d0751ce99e9c72525232bb15eb42c9be11"))
            (      2, uint256S("0xe06981dbcea41455f7ddda037dbee1431337d5999d988d266fcdcb51e2bdaa71")),

            1458054397, // * UNIX timestamp of last checkpoint block
            2,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1600    // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
private:
    Consensus::Params minDifficultyConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";

        // Blocks 0 - 20000 are like main-net
        consensus.nHeightEffective = 0;
        consensus.nPowTargetTimespan = 60;
        consensus.fDigishieldDifficultyCalculation = true;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.nMajorityEnforceBlockUpgrade = 501;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        consensus.fStrictChainId = false;
        consensus.nHeightEffective = 0;
        consensus.fAllowLegacyBlocks = true;
        consensus.fAllowAuxPow = false;

        // Reset links before we copy parameters
        consensus.pLeft = NULL;
        consensus.pRight = NULL;

        // Blocks 20000+ are Digishield with minimum difficulty on all blocks
        digishieldConsensus = consensus;
        minDifficultyConsensus = consensus;
        minDifficultyConsensus.nHeightEffective = 20000;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = true;
        
        // Assemble the binary search tree of parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &minDifficultyConsensus;
        minDifficultyConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        vAlertPubKey = ParseHex("042756726da3c7ef515d89212ee1705023d14be389e25fe15611585661b9a20021908b2b80a3c7200a0139dd2b26946606aab0eef9aa7689a6dc2c7eee237fa834");
        nDefaultPort = 22335;
        nMinerThreads = 0;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1455393600;
        genesis.nNonce = 571735;
        
        // If genesis block hash does not match, then generate new genesis hash.
        if (false) {
        printf("Searching for %s genesis block...\n", strNetworkID.c_str());
        // This will figure out a valid hash and Nonce if you're
        // creating a different genesis block:
        arith_uint256 hashTarget = UintToArith256(consensus.powLimit);
        while(true) {
        arith_uint256 thash = UintToArith256(genesis.GetPoWHash());
        if (thash <= hashTarget)
            break;
        if ((genesis.nNonce & 0xFFF) == 0) {
        printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
        printf("NONCE WRAPPED, incrementing time\n");
        ++genesis.nTime;
        }
        }
        printf("%s genesis.nTime = %u \n", strNetworkID.c_str(), genesis.nTime);
        printf("%s genesis.nNonce = %u \n", strNetworkID.c_str(), genesis.nNonce);
        printf("%s genesis.GetPoWHash = %s\n", strNetworkID.c_str(), genesis.GetPoWHash().ToString().c_str());
        printf("%s genesis.GetHash = %s\n", strNetworkID.c_str(), genesis.GetHash().ToString().c_str());
        printf("%s genesis.hashMerkleRoot = %s\n", strNetworkID.c_str(), genesis.hashMerkleRoot.ToString().c_str());
        }
        
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0xa47faf8f528948fe87369500a427f9cff5a6b3706dc34e8cee6ea4492453d974"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("jrn.me.uk", "testseed.jrn.me.uk"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,113); // 0x71
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,241); // 0xf1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xcf).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        //TODO: fix this for lemoncoin -- plddr
        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x2ece70050fe38c471b3a9276d94eb123ce81f0cd44f7402a4efc1905faee52b5")),
          //( 483173, uint256S("0xa804201ca0aceb7e937ef7a3c613a9b7589245b10cc095148c4ce4965b0b73b5"))
          //( 591117, uint256S("0x5f6b93b2c28cedf32467d900369b8be6700f0649388a7dbfd3ebd4a01b1ffad8"))
            1455393600, // * UNIX timestamp of last checkpoint block
            0,    // * total number of transactions between genesis and last checkpoint
            1000        // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
private:
    Consensus::Params digishieldConsensus;
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 1; // regtest: 1 second blocks
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1;
        consensus.fStrictChainId = false;
        consensus.fAllowLegacyBlocks = true;
        consensus.fAllowAuxPow = false;
        consensus.nCoinbaseMaturity = 20;

        // Reset links before we copy parameters
        consensus.pLeft = NULL;
        consensus.pRight = NULL;

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 10;
        digishieldConsensus.nPowTargetTimespan = 1; // // regtest: also retarget every second in digishield mode, for conformity
        digishieldConsensus.fDigishieldDifficultyCalculation = false;
        
        // Assemble the binary search tree of parameters
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        pConsensusRoot = &digishieldConsensus;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nMinerThreads = 1;
        genesis.nTime = 1455393600;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 5;
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        nDefaultPort = 22336;
        
        // If genesis block hash does not match, then generate new genesis hash.
        if (false) {
        printf("Searching for %s genesis block...\n", strNetworkID.c_str());
        // This will figure out a valid hash and Nonce if you're
        // creating a different genesis block:
        arith_uint256 hashTarget = UintToArith256(consensus.powLimit);
        while(true) {
        arith_uint256 thash = UintToArith256(genesis.GetPoWHash());
        if (thash <= hashTarget)
            break;
        if ((genesis.nNonce & 0xFFF) == 0) {
        printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
        printf("NONCE WRAPPED, incrementing time\n");
        ++genesis.nTime;
        }
        }
        printf("%s genesis.nTime = %u \n", strNetworkID.c_str(), genesis.nTime);
        printf("%s genesis.nNonce = %u \n", strNetworkID.c_str(), genesis.nNonce);
        printf("%s genesis.GetPoWHash = %s\n", strNetworkID.c_str(), genesis.GetPoWHash().ToString().c_str());
        printf("%s genesis.GetHash = %s\n", strNetworkID.c_str(), genesis.GetHash().ToString().c_str());
        printf("%s genesis.hashMerkleRoot = %s\n", strNetworkID.c_str(), genesis.hashMerkleRoot.ToString().c_str());
        }
        assert(consensus.hashGenesisBlock == uint256S("0x8a11ee39807498cf33dc05c96d1516a6de871b73e249c9b179b1daf4b991b891"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x8a11ee39807498cf33dc05c96d1516a6de871b73e249c9b179b1daf4b991b891")),
            0,
            0,
            0
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
/*
        // If genesis block hash does not match, then generate new genesis hash.
        if (true) {
        printf("Searching for %s genesis block...\n", strNetworkID);
        // This will figure out a valid hash and Nonce if you're
        // creating a different genesis block:
        arith_uint256 hashTarget = UintToArith256(consensus.powLimit);
        while(true) {
        arith_uint256 thash = UintToArith256(genesis.GetPoWHash());
        if (thash <= hashTarget)
            break;
        if ((genesis.nNonce & 0xFFF) == 0) {
        printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0) {
        printf("NONCE WRAPPED, incrementing time\n");
        ++genesis.nTime;
        }
        }
        printf("%s genesis.nTime = %u \n", strNetworkID, genesis.nTime);
        printf("%s genesis.nNonce = %u \n", strNetworkID, genesis.nNonce);
        printf("%s genesis.GetPoWHash = %s\n", strNetworkID, genesis.GetPoWHash().ToString().c_str());
        printf("%s genesis.GetHash = %s\n", strNetworkID, genesis.GetHash().ToString().c_str());
        printf("%s genesis.hashMerkleRoot = %s\n", strNetworkID, genesis.hashMerkleRoot.ToString().c_str());
        }
*/