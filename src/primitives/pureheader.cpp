// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2016 The MMXVI developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/pureheader.h"

#include "chainparams.h"
#include "crypto/scrypt.h"
#include "hash.h"
#include "utilstrencodings.h"

uint256 CPureBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CPureBlockHeader::GetPoWHash() const
{
    return hashArgon2(BEGIN(nVersion), END(nNonce));
}
