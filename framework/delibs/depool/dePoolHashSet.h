#ifndef _DEPOOLHASHSET_H
#define _DEPOOLHASHSET_H
/*-------------------------------------------------------------------------
 * drawElements Memory Pool Library
 * --------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief Memory pool hash-set class.
 *//*--------------------------------------------------------------------*/

#include "deDefs.h"
#include "dePoolHash.h"
#include "dePoolSet.h"

DE_BEGIN_EXTERN_C

void	dePoolHashSet_selfTest		(void);

DE_END_EXTERN_C

/*--------------------------------------------------------------------*//*!
 * \brief Declare a template pool hash-set (hash of sets) class interface.
 * \param TYPENAME	Type name of the declared hash-set.
 * \param KEYTYPE	Type of the key.
 * \param VALUETYPE	Type of the value.
 *
 * \todo [petri] Description.
 *
 * The functions for operating the hash are:
 * \todo [petri] Figure out how to comment these in Doxygen-style.
 *
 * \code
 * HashSet*    HashSet_create            (deMemPool* pool);
 * int         HashSet_getNumElements    (const HashSet* hashSet);
 * Set<Value>* HashSet_find              (const HashSet* hashSet, Key key); TODO: better API
 * Hash<Set*>* HashSet_getHash           (const HashSet* hashSet); TODO: better API
 * deBool      HashSet_insert            (HashSet* hashSet, Key key, Value value);
 * void        HashSet_delete            (HashSet* hashSet, Key key, Value value);
 * deBool      HashSet_exists            (const HashSet* hashSet, Key key, Value value);
 * \endcode
*//*--------------------------------------------------------------------*/
#define DE_DECLARE_POOL_HASH_SET(TYPENAME, KEYTYPE, VALUETYPE)										\
																									\
DE_DECLARE_POOL_SET(TYPENAME##Set, VALUETYPE);														\
DE_DECLARE_POOL_HASH(TYPENAME##Hash, KEYTYPE, TYPENAME##Set*);										\
typedef struct TYPENAME##_s																			\
{																									\
	TYPENAME##Hash*	hash;																			\
} TYPENAME;																							\
																									\
DE_INLINE TYPENAME*			TYPENAME##_create			(deMemPool* pool);							\
DE_INLINE int				TYPENAME##_getNumElements	(const TYPENAME* hashSet)								DE_UNUSED_FUNCTION;	\
DE_INLINE TYPENAME##Hash*	TYPENAME##_getHash			(const TYPENAME* hashSet)								DE_UNUSED_FUNCTION;	\
DE_INLINE deBool			TYPENAME##_insert			(TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)		DE_UNUSED_FUNCTION;	\
DE_INLINE deBool			TYPENAME##_safeInsert		(TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)		DE_UNUSED_FUNCTION;	\
DE_INLINE TYPENAME##Set*	TYPENAME##_find				(const TYPENAME* hashSet, KEYTYPE key)					DE_UNUSED_FUNCTION;	\
DE_INLINE void				TYPENAME##_delete			(TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)		DE_UNUSED_FUNCTION;	\
DE_INLINE deBool			TYPENAME##_exists			(const TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)	DE_UNUSED_FUNCTION;	\
																									\
DE_INLINE TYPENAME* TYPENAME##_create (deMemPool* pool)												\
{																									\
	TYPENAME* hashSet = DE_POOL_NEW(pool, TYPENAME);												\
	if (!hashSet) return DE_NULL;																	\
	if ((hashSet->hash = TYPENAME##Hash_create(pool)) == DE_NULL)									\
		return DE_NULL;																				\
	return hashSet;																					\
}																									\
																									\
DE_INLINE int TYPENAME##_getNumElements (const TYPENAME* hashSet)									\
{																									\
	return TYPENAME##Hash_getNumElements(hashSet->hash);											\
}																									\
																									\
DE_INLINE TYPENAME##Hash* TYPENAME##_getHash (const TYPENAME* hashSet)								\
{																									\
	return hashSet->hash;																			\
}																									\
																									\
DE_INLINE deBool TYPENAME##_insert (TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)				\
{																									\
	TYPENAME##Set**	setPtr	= TYPENAME##Hash_find(hashSet->hash, key);								\
	TYPENAME##Set*	set		= setPtr ? *setPtr : DE_NULL;											\
	if (!set)																						\
	{																								\
		set = TYPENAME##Set_create(hashSet->hash->pool);											\
		if (!set) return DE_FALSE;																	\
		if (!TYPENAME##Set_insert(set, value)) return DE_FALSE;										\
		return TYPENAME##Hash_insert(hashSet->hash, key, set);										\
	}																								\
	else																							\
	{																								\
		return TYPENAME##Set_insert(set, value);													\
	}																								\
}																									\
																									\
DE_INLINE deBool TYPENAME##_safeInsert (TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)			\
{																									\
	TYPENAME##Set**	setPtr	= TYPENAME##Hash_find(hashSet->hash, key);								\
	TYPENAME##Set*	set		= setPtr ? *setPtr : DE_NULL;											\
	if (!set)																						\
	{																								\
		return TYPENAME##_insert(hashSet, key, value);												\
	}																								\
	else																							\
	{																								\
		return TYPENAME##Set_safeInsert(set, value);												\
	}																								\
}																									\
																									\
DE_INLINE TYPENAME##Set* TYPENAME##_find (const TYPENAME* hashSet, KEYTYPE key)						\
{																									\
	TYPENAME##Set** setPtr = TYPENAME##Hash_find(hashSet->hash, key);								\
	return setPtr ? *setPtr : DE_NULL;																\
}																									\
																									\
DE_INLINE void TYPENAME##_delete (TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)					\
{																									\
	TYPENAME##Set**	setPtr = TYPENAME##Hash_find(hashSet->hash, key);								\
	TYPENAME##Set*	set;																			\
	DE_ASSERT(setPtr);																				\
	set = *setPtr;																					\
	TYPENAME##Set_delete(set, value);																\
}																									\
																									\
DE_INLINE deBool TYPENAME##_exists (const TYPENAME* hashSet, KEYTYPE key, VALUETYPE value)			\
{																									\
	TYPENAME##Set** setPtr = TYPENAME##Hash_find(hashSet->hash, key);								\
	if (setPtr)																						\
		return TYPENAME##Set_exists(*setPtr, value);												\
	else																							\
		return DE_FALSE;																			\
}																									\
																									\
struct TYPENAME##Dummy_s { int dummy; }

/*--------------------------------------------------------------------*//*!
 * \brief Implement a template pool hash-set class.
 * \param TYPENAME	Type name of the declared hash.
 * \param KEYTYPE	Type of the key.
 * \param VALUETYPE	Type of the value.
 * \param HASHFUNC	Function used for hashing the key.
 * \param CMPFUNC	Function used for exact matching of the keys.
 *
 * This macro has implements the hash declared with DE_DECLARE_POOL_HASH.
 * Usually this macro should be used from a .c file, since the macro expands
 * into multiple functions. The TYPENAME, KEYTYPE, and VALUETYPE parameters
 * must match those of the declare macro.
*//*--------------------------------------------------------------------*/
#define DE_IMPLEMENT_POOL_HASH_SET(TYPENAME, KEYTYPE, VALUETYPE, KEYHASHFUNC, KEYCMPFUNC, VALUEHASHFUNC, VALUECMPFUNC)	\
DE_IMPLEMENT_POOL_SET(TYPENAME##Set, VALUETYPE, VALUEHASHFUNC, VALUECMPFUNC);											\
DE_IMPLEMENT_POOL_HASH(TYPENAME##Hash, KEYTYPE, TYPENAME##Set*, KEYHASHFUNC, KEYCMPFUNC);								\
struct TYPENAME##Dummy2_s { int dummy; }

/* Copy-to-array templates. */

#if 0

#define DE_DECLARE_POOL_HASH_TO_ARRAY(HASHTYPENAME, KEYARRAYTYPENAME, VALUEARRAYTYPENAME)		\
	deBool HASHTYPENAME##_copyToArray(const HASHTYPENAME* set, KEYARRAYTYPENAME* keyArray, VALUEARRAYTYPENAME* valueArray);	\
	struct HASHTYPENAME##_##KEYARRAYTYPENAME##_##VALUEARRAYTYPENAME##_declare_dummy { int dummy; }

#define DE_IMPLEMENT_POOL_HASH_TO_ARRAY(HASHTYPENAME, KEYARRAYTYPENAME, VALUEARRAYTYPENAME)		\
deBool HASHTYPENAME##_copyToArray(const HASHTYPENAME* hash, KEYARRAYTYPENAME* keyArray, VALUEARRAYTYPENAME* valueArray)	\
{	\
	int numElements	= hash->numElements;	\
	int arrayNdx	= 0;	\
	int slotNdx;	\
	\
	if ((keyArray && !KEYARRAYTYPENAME##_setSize(keyArray, numElements)) ||			\
		(valueArray && !VALUEARRAYTYPENAME##_setSize(valueArray, numElements)))		\
		return DE_FALSE;	\
	\
	for (slotNdx = 0; slotNdx < hash->slotTableSize; slotNdx++) \
	{ \
		const HASHTYPENAME##Slot* slot = hash->slotTable[slotNdx]; \
		while (slot) \
		{ \
			int elemNdx; \
			for (elemNdx = 0; elemNdx < slot->numUsed; elemNdx++) \
			{	\
				if (keyArray)	\
					KEYARRAYTYPENAME##_set(keyArray, arrayNdx, slot->keys[elemNdx]); \
				if (valueArray)	\
					VALUEARRAYTYPENAME##_set(valueArray, arrayNdx, slot->values[elemNdx]);	\
				arrayNdx++;	\
			} \
			slot = slot->nextSlot; \
		} \
	}	\
	DE_ASSERT(arrayNdx == numElements);	\
	return DE_TRUE;	\
}	\
struct HASHTYPENAME##_##KEYARRAYTYPENAME##_##VALUEARRAYTYPENAME##_implement_dummy { int dummy; }

#endif

#endif /* _DEPOOLHASHSET_H */
