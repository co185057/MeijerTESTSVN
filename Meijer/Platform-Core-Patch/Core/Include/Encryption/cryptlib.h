// cryptlib.h - written and placed in the public domain by Wei Dai
/*! \file
 	This file contains the declarations for the abstract base
	classes that provide a uniform interface to this library.
*/

/*!	\mainpage <a href="http://www.cryptopp.com">Crypto++</a><sup><small>TM</small></sup> Library 5.0.4 Reference Manual
<dl>
<dt>Abstract Base Classes<dd>
	cryptlib.h
<dt>Symmetric Ciphers<dd>
	SymmetricCipherDocumentation
<dt>Hash Functions<dd>
	HAVAL, MD2, MD4, MD5, PanamaHash, RIPEMD160, SHA, SHA256, SHA384, SHA512, Tiger
<dt>Non-Cryptographic Checksums<dd>
	CRC32, Adler32
<dt>Message Authentication Codes<dd>
	#MD5MAC, XMACC, HMAC, CBC_MAC, DMAC, PanamaMAC
<dt>Random Number Generators<dd>
	NullRNG(), LC_RNG, RandomPool, BlockingRng, NonblockingRng, AutoSeededRandomPool, AutoSeededX917RNG
<dt>Public Key Cryptosystems<dd>
	DLIES, ECIES, LUCES, RSAES, RabinES, LUC_IES
<dt>Public Key Signature Schemes<dd>
	DSA, GDSA, ECDSA, NR, ECNR, LUCSSA, RSASSA, RabinSSR, RWSSA, ESIGN
<dt>Key Agreement<dd>
	#DH, DH2, #MQV, ECDH, ECMQV, XTR_DH
<dt>Algebraic Structures<dd>
	Integer, PolynomialMod2, PolynomialOver, RingOfPolynomialsOver,
	ModularArithmetic, MontgomeryRepresentation, GFP2_ONB,
	GF2NP, GF256, GF2_32, EC2N, ECP
<dt>Secret Sharing and Information Dispersal<dd>
	SecretSharing, SecretRecovery, InformationDispersal, InformationRecovery
<dt>Compression<dd>
	Deflator, Inflator, Gzip, Gunzip, ZlibCompressor, ZlibDecompressor
<dt>Input Source Classes<dd>
	StringSource, FileSource, SocketSource, WindowsPipeSource, RandomNumberSource
<dt>Output Sink Classes<dd>
	StringSinkTemplate, ArraySink, FileSink, SocketSink, WindowsPipeSink
<dt>Filter Wrappers<dd>
	StreamTransformationFilter, HashFilter, HashVerificationFilter, SignerFilter, SignatureVerificationFilter
<dt>Binary to Text Encoders and Decoders<dd>
	HexEncoder, HexDecoder, Base64Encoder, Base64Decoder
<dt>Wrappers for OS features<dd>
	Timer, Socket, WindowsHandle, ThreadLocalStorage
<dt>FIPS 140 related<dd>
	fips140.h
</dl>

In the FIPS 140-2 validated DLL version of Crypto++, only the following 
security function implementation classes are available.
<dl>
<dt>Block Ciphers<dd>
	AES, DES_EDE2, DES_EDE3, SKIPJACK
<dt>Cipher Modes (replace template parameter BC with one of the block ciphers above)<dd>
	ECB_Mode <BC>, CTR_Mode <BC>, CBC_Mode <BC>, CFB_Mode <BC>, OFB_Mode <BC>
<dt>Hash Functions<dd>
	SHA
<dt>Public Key Signature Schemes<dd>
	RSASSA <PKCS1v15, SHA>, DSA, ECDSA <ECP, SHA>, ECDSA <EC2N, SHA>
<dt>Message Authentication Codes<dd>
	HMAC <SHA>, CBC_MAC <DES_EDE2>, CBC_MAC <DES_EDE3>
<dt>Random Number Generators<dd>
	AutoSeededX917RNG <DES_EDE3>
<dt>Key Agreement<dd>
	#DH
<dt>Public Key Cryptosystems<dd>
	RSAES <OAEP<SHA> >
</dl>

<p>This reference manual is a work in progress. Some classes are still lacking detailed descriptions.
<p>Click <a href="CryptoPPRef.zip">here</a> to download a zip archive containing this manual.
<p>Thanks to Ryan Phillips for providing the Doxygen configuration file
and getting me started with this manual.
*/

#ifndef CRYPTOPP_CRYPTLIB_H
#define CRYPTOPP_CRYPTLIB_H

#include "config.h"
#include "stdcpp.h"

NAMESPACE_BEGIN(CryptoPP)

// forward declarations
class Integer;

//! used to specify a direction for a cipher to operate in (encrypt or decrypt)
enum CipherDir {ENCRYPTION,	DECRYPTION};

//! used to represent infinite time
const unsigned long INFINITE_TIME = ULONG_MAX;

// VC60 workaround: using enums as template parameters causes problems
template <typename ENUM_TYPE, int VALUE>
struct EnumToType
{
	static ENUM_TYPE ToEnum() {return (ENUM_TYPE)VALUE;}
};

enum ByteOrder {LITTLE_ENDIAN_ORDER = 0, BIG_ENDIAN_ORDER = 1};
typedef EnumToType<ByteOrder, LITTLE_ENDIAN_ORDER> LittleEndian;
typedef EnumToType<ByteOrder, BIG_ENDIAN_ORDER> BigEndian;

//! base class for all exceptions thrown by Crypto++
class CRYPTOPP_DLL Exception : public std::exception
{
public:
	//! error types
	enum ErrorType {
		//! a method is not implemented
		NOT_IMPLEMENTED,
		//! invalid function argument
		INVALID_ARGUMENT,
		//! BufferedTransformation received a Flush(true) signal but can't flush buffers
		CANNOT_FLUSH,
		//! data integerity check (such as CRC or MAC) failed
		DATA_INTEGRITY_CHECK_FAILED,
		//! received input data that doesn't conform to expected format
		INVALID_DATA_FORMAT,
		//! error reading from input device or writing to output device
		IO_ERROR,
		//! some error not belong to any of the above categories
		OTHER_ERROR
	};

	explicit Exception(ErrorType errorType, const std::string &s) : m_what(s) {}
	virtual ~Exception() throw() {}
	const char *what() const throw() {return (m_what.c_str());}
	const std::string &GetWhat() const {return m_what;}
	void SetWhat(const std::string &s) {m_what = s;}
	ErrorType GetErrorType() const {return m_errorType;}
	void SetErrorType(ErrorType errorType) {m_errorType = errorType;}

private:
	ErrorType m_errorType;
	std::string m_what;
};

//! exception thrown when an invalid argument is detected
class CRYPTOPP_DLL InvalidArgument : public Exception
{
public:
	explicit InvalidArgument(const std::string &s) : Exception(INVALID_ARGUMENT, s) {}
};

//! exception thrown by decryption filters when trying to decrypt an invalid ciphertext
class CRYPTOPP_DLL InvalidDataFormat : public Exception
{
public:
	explicit InvalidDataFormat(const std::string &s) : Exception(INVALID_DATA_FORMAT, s) {}
};

//! exception thrown by decryption filters when trying to decrypt an invalid ciphertext
class CRYPTOPP_DLL InvalidCiphertext : public InvalidDataFormat
{
public:
	explicit InvalidCiphertext(const std::string &s) : InvalidDataFormat(s) {}
};

//! exception thrown by a class if a non-implemented method is called
class CRYPTOPP_DLL NotImplemented : public Exception
{
public:
	explicit NotImplemented(const std::string &s) : Exception(NOT_IMPLEMENTED, s) {}
};

//! exception thrown by a class when Flush(true) is called but it can't completely flush its buffers
class CRYPTOPP_DLL CannotFlush : public Exception
{
public:
	explicit CannotFlush(const std::string &s) : Exception(CANNOT_FLUSH, s) {}
};

//! error reported by the operating system
class CRYPTOPP_DLL OS_Error : public Exception
{
public:
	OS_Error(ErrorType errorType, const std::string s, const std::string& operation, int errorCode)
		: Exception(errorType, s), m_operation(operation), m_errorCode(errorCode) {}
	~OS_Error() throw() {}

	// the operating system API that reported the error
	const std::string & GetOperation() const {return m_operation;}
	// the error code return by the operating system
	int GetErrorCode() const {return m_errorCode;}

protected:
	std::string m_operation;
	int m_errorCode;
};

//! used to return decoding results
struct CRYPTOPP_DLL DecodingResult
{
	explicit DecodingResult() : isValidCoding(false), messageLength(0) {}
	explicit DecodingResult(unsigned int len) : isValidCoding(true), messageLength(len) {}

	bool operator==(const DecodingResult &rhs) const {return isValidCoding == rhs.isValidCoding && messageLength == rhs.messageLength;}
	bool operator!=(const DecodingResult &rhs) const {return !operator==(rhs);}

	bool isValidCoding;
	unsigned int messageLength;

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
	operator unsigned int() const {return isValidCoding ? messageLength : 0;}
#endif
};

//! interface for retrieving values given their names
/*! This class is used to safely pass a variable number of arbitrarily typed arguments to functions
	and to read values from keys and crypto parameters.
	To get a value, you need to know the name and the type of the value. 
	Call GetValueNames() on a NameValuePairs object to obtain a list of value names that it supports.
	Then look at the Name namespace documentation to see what the type of each value is, or
	alternatively, call GetIntValue() with the value name, and if the type is not int, a
	ValueTypeMismatch exception will be thrown and you can get the actual type from the exception object.
*/
class NameValuePairs
{
public:
	virtual ~NameValuePairs() {}

	//! exception thrown when trying to retrieve a value using a different type than expected
	class ValueTypeMismatch : public InvalidArgument
	{
	public:
		ValueTypeMismatch(std::string name, const std::type_info &stored, const std::type_info &retrieving)
			: InvalidArgument("NameValuePairs: type mismatch for '" + name + "', stored '" + stored.name() + "', trying to retrieve '" + retrieving.name() + "'")
			, m_stored(stored), m_retrieving(retrieving) {}

		const std::type_info & GetStoredTypeInfo() const {return m_stored;}
		const std::type_info & GetRetrievingTypeInfo() const {return m_retrieving;}

	private:
		const std::type_info &m_stored;
		const std::type_info &m_retrieving;
	};

	//! get a copy of this object or a subobject of it
	template <class T>
	bool GetThisObject(T &object) const
	{
		return GetValue((std::string("ThisObject:")+typeid(T).name()).c_str(), object);
	}

	//! get a pointer to this object, as a pointer to T
	template <class T>
	bool GetThisPointer(T *&p) const
	{
		return GetValue((std::string("ThisPointer:")+typeid(T).name()).c_str(), p);
	}

	//! get a named value, returns true if the name exists
	template <class T>
	bool GetValue(const char *name, T &value) const
	{
		return GetVoidValue(name, typeid(T), &value);
	}

	//! get a named value, returns the default if the name doesn't exist
	template <class T>
	T GetValueWithDefault(const char *name, T defaultValue) const
	{
		GetValue(name, defaultValue);
		return defaultValue;
	}

	//! get a list of value names that can be retrieved
	CRYPTOPP_DLL std::string GetValueNames() const
		{std::string result; GetValue("ValueNames", result); return result;}

	//! get a named value with type int
	/*! used to ensure we don't accidentally try to get an unsigned int
		or some other type when we mean int (which is the most common case) */
	CRYPTOPP_DLL bool GetIntValue(const char *name, int &value) const
		{return GetValue(name, value);}

	//! get a named value with type int, with default
	CRYPTOPP_DLL int GetIntValueWithDefault(const char *name, int defaultValue) const
		{return GetValueWithDefault(name, defaultValue);}

	//! used by derived classes to check for type mismatch
	CRYPTOPP_DLL static void ThrowIfTypeMismatch(const char *name, const std::type_info &stored, const std::type_info &retrieving)
		{if (stored != retrieving) throw ValueTypeMismatch(name, stored, retrieving);}

	template <class T>
	void GetRequiredParameter(const char *className, const char *name, T &value) const
	{
		if (!GetValue(name, value))
			throw InvalidArgument(std::string(className) + ": missing required parameter '" + name + "'");
	}

	CRYPTOPP_DLL void GetRequiredIntParameter(const char *className, const char *name, int &value) const
	{
		if (!GetIntValue(name, value))
			throw InvalidArgument(std::string(className) + ": missing required parameter '" + name + "'");
	}

	//! to be implemented by derived classes, users should use one of the above functions instead
	CRYPTOPP_DLL virtual bool GetVoidValue(const char *name, const std::type_info &valueType, void *pValue) const =0;
};

//! namespace containing value name definitions
/*!	value names, types and semantics:

	ThisObject:ClassName (ClassName, copy of this object or a subobject)
	ThisPointer:ClassName (const ClassName *, pointer to this object or a subobject)
*/
DOCUMENTED_NAMESPACE_BEGIN(Name)
// more names defined in argnames.h
DOCUMENTED_NAMESPACE_END

//! .
class CRYPTOPP_DLL NullNameValuePairs : public NameValuePairs
{
public:
	bool GetVoidValue(const char *name, const std::type_info &valueType, void *pValue) const {return false;}
};

//! .
extern CRYPTOPP_DLL const NullNameValuePairs g_nullNameValuePairs;

// ********************************************************

//! interface for cloning objects, this is not implemented by most classes yet
class CRYPTOPP_DLL Clonable
{
public:
	virtual ~Clonable() {}
	//! this is not implemented by most classes yet
	virtual Clonable* Clone() const {throw NotImplemented("Clone() is not implemented yet.");}	// TODO: make this =0
};

//! interface for all crypto algorithms

class CRYPTOPP_DLL Algorithm : public Clonable
{
public:
	/*! When FIPS-140-2 compliance is enabled and checkSelfTestStatus == true,
		this constructor throws SelfTestFailure if the self test hasn't been run or fails. */
	Algorithm(bool checkSelfTestStatus = true);
	//! returns name of this algorithm, not universally implemented yet
	virtual std::string AlgorithmName() const {return "unknown";}
};

//! keying interface for crypto algorithms that take byte strings as keys

class CRYPTOPP_DLL SimpleKeyingInterface
{
public:
	//! returns smallest valid key length in bytes */
	virtual unsigned int MinKeyLength() const =0;
	//! returns largest valid key length in bytes */
	virtual unsigned int MaxKeyLength() const =0;
	//! returns default (recommended) key length in bytes */
	virtual unsigned int DefaultKeyLength() const =0;

	//! returns the smallest valid key length in bytes that is >= min(n, GetMaxKeyLength())
	virtual unsigned int GetValidKeyLength(unsigned int n) const =0;

	//! returns whether n is a valid key length
	virtual bool IsValidKeyLength(unsigned int n) const
		{return n == GetValidKeyLength(n);}

	//! set or reset the key of this object
	/*! \param params is used to specify Rounds, BlockSize, etc */
	virtual void SetKey(const byte *key, unsigned int length, const NameValuePairs &params = g_nullNameValuePairs) =0;

	//! calls SetKey() with an NameValuePairs object that just specifies "Rounds"
	void SetKeyWithRounds(const byte *key, unsigned int length, int rounds);

	//! calls SetKey() with an NameValuePairs object that just specifies "IV"
	void SetKeyWithIV(const byte *key, unsigned int length, const byte *iv);

	enum IV_Requirement {STRUCTURED_IV = 0, RANDOM_IV, UNPREDICTABLE_RANDOM_IV, INTERNALLY_GENERATED_IV, NOT_RESYNCHRONIZABLE};
	//! returns the minimal requirement for secure IVs
	virtual IV_Requirement IVRequirement() const =0;

	//! returns whether this object can be resynchronized (i.e. supports initialization vectors)
	/*! If this function returns true, and no IV is passed to SetKey() and CanUseStructuredIVs()==true, an IV of all 0's will be assumed. */
	bool IsResynchronizable() const {return IVRequirement() < NOT_RESYNCHRONIZABLE;}
	//! returns whether this object can use random IVs (in addition to ones returned by GetNextIV)
	bool CanUseRandomIVs() const {return IVRequirement() <= UNPREDICTABLE_RANDOM_IV;}
	//! returns whether this object can use random but possibly predictable IVs (in addition to ones returned by GetNextIV)
	bool CanUsePredictableIVs() const {return IVRequirement() <= RANDOM_IV;}
	//! returns whether this object can use structured IVs, for example a counter (in addition to ones returned by GetNextIV)
	bool CanUseStructuredIVs() const {return IVRequirement() <= STRUCTURED_IV;}

	//! returns size of IVs used by this object
	virtual unsigned int IVSize() const {throw NotImplemented("SimpleKeyingInterface: this object doesn't support resynchronization");}
	//! resynchronize with an IV
	virtual void Resynchronize(const byte *IV) {throw NotImplemented("SimpleKeyingInterface: this object doesn't support resynchronization");}
	//! get a secure IV for the next message
	/*! This method should be called after you finish encrypting one message and are ready to start the next one.
		After calling it, you must call SetKey() or Resynchronize() before using this object again. 
		This method is not implemented on decryption objects. */
	virtual void GetNextIV(byte *IV) {throw NotImplemented("SimpleKeyingInterface: this object doesn't support GetNextIV()");}

protected:
	void ThrowIfInvalidKeyLength(const Algorithm &algorithm, unsigned int length);

	inline void AssertValidKeyLength(unsigned int length) const
	{
		assert(IsValidKeyLength(length));
	}
};

//! interface for the data processing part of block ciphers

/*! Classes derived from BlockTransformation are block ciphers
	in ECB mode (for example the DES::Encryption class), which are stateless,
	and they can make assumptions about the memory alignment of their inputs and outputs.
	These classes should not be used directly, but only in combination with
	a mode class (see CipherModeDocumentation in modes.h).
*/
class CRYPTOPP_DLL BlockTransformation : public Algorithm
{
public:
	//! encrypt or decrypt inBlock, xor with xorBlock, and write to outBlock
	virtual void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const =0;

	//! encrypt or decrypt one block
	/*! \pre size of inBlock and outBlock == BlockSize() */
	void ProcessBlock(const byte *inBlock, byte *outBlock) const
		{ProcessAndXorBlock(inBlock, NULL, outBlock);}

	//! encrypt or decrypt one block in place
	void ProcessBlock(byte *inoutBlock) const
		{ProcessAndXorBlock(inoutBlock, NULL, inoutBlock);}

	//! block size of the cipher in bytes
	virtual unsigned int BlockSize() const =0;

	//! block pointers must be divisible by this
	virtual unsigned int BlockAlignment() const {return 4;}

	//! returns true if this is a permutation (i.e. there is an inverse transformation)
	virtual bool IsPermutation() const {return true;}

	//! returns true if this is an encryption object
	virtual bool IsForwardTransformation() const =0;

	//! return number of blocks that can be processed in parallel, for bit-slicing implementations
	virtual unsigned int OptimalNumberOfParallelBlocks() const {return 1;}

	//! encrypt or decrypt multiple blocks, for bit-slicing implementations
	virtual void ProcessAndXorMultipleBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, unsigned int numberOfBlocks) const;
};

//! interface for the data processing part of stream ciphers

class CRYPTOPP_DLL StreamTransformation : public Algorithm
{
public:
	//! return a reference to this object, 
	/*! This function is useful for passing a temporary StreamTransformation object to a 
		function that takes a non-const reference. */
	StreamTransformation& Ref() {return *this;}

	//! returns block size, if input must be processed in blocks, otherwise 1
	virtual unsigned int MandatoryBlockSize() const {return 1;}

	//! returns the input block size that is most efficient for this cipher
	/*! \note optimal input length is n * OptimalBlockSize() - GetOptimalBlockSizeUsed() for any n > 0 */
	virtual unsigned int OptimalBlockSize() const {return MandatoryBlockSize();}
	//! returns how much of the current block is used up
	virtual unsigned int GetOptimalBlockSizeUsed() const {return 0;}

	//! returns how input should be aligned for optimal performance
	virtual unsigned int OptimalDataAlignment() const {return 1;}

	//! encrypt or decrypt an array of bytes of specified length
	/*! \note either inString == outString, or they don't overlap */
	virtual void ProcessData(byte *outString, const byte *inString, unsigned int length) =0;

	//! for ciphers where the last block of data is special, encrypt or decrypt the last block of data
	/*! For now the only use of this function is for CBC-CTS mode. */
	virtual void ProcessLastBlock(byte *outString, const byte *inString, unsigned int length);
	//! returns the minimum size of the last block, 0 indicating the last block is not special
	virtual unsigned int MinLastBlockSize() const {return 0;}

	//! same as ProcessData(inoutString, inoutString, length)
	inline void ProcessString(byte *inoutString, unsigned int length)
		{ProcessData(inoutString, inoutString, length);}
	//! same as ProcessData(outString, inString, length)
	inline void ProcessString(byte *outString, const byte *inString, unsigned int length)
		{ProcessData(outString, inString, length);}
	//! implemented as {ProcessData(&input, &input, 1); return input;}
	inline byte ProcessByte(byte input)
		{ProcessData(&input, &input, 1); return input;}

	//! returns whether this cipher supports random access
	virtual bool IsRandomAccess() const =0;
	//! for random access ciphers, seek to an absolute position
	virtual void Seek(dword n)
	{
		assert(!IsRandomAccess());
		throw NotImplemented("StreamTransformation: this object doesn't support random access");
	}

	//! returns whether this transformation is self-inverting (e.g. xor with a keystream)
	virtual bool IsSelfInverting() const =0;
	//! returns whether this is an encryption object
	virtual bool IsForwardTransformation() const =0;
};

//! interface for hash functions and data processing part of MACs

/*! HashTransformation objects are stateful.  They are created in an initial state,
	change state as Update() is called, and return to the initial
	state when Final() is called.  This interface allows a large message to
	be hashed in pieces by calling Update() on each piece followed by
	calling Final().
*/
class CRYPTOPP_DLL HashTransformation : public Algorithm
{
public:
	//! process more input
	virtual void Update(const byte *input, unsigned int length) =0;

	//! request space to write input into
	virtual byte * CreateUpdateSpace(unsigned int &size) {size=0; return NULL;}

	//! compute hash for current message, then restart for a new message
	/*!	\pre size of digest == DigestSize(). */
	virtual void Final(byte *digest)
		{TruncatedFinal(digest, DigestSize());}

	//! discard the current state, and restart with a new message
	virtual void Restart()
		{TruncatedFinal(NULL, 0);}

	//! size of the hash returned by Final()
	virtual unsigned int DigestSize() const =0;

	//! block size of underlying compression function, or 0 if not block based
	virtual unsigned int BlockSize() const {return 0;}

	//! input to Update() should have length a multiple of this for optimal speed
	virtual unsigned int OptimalBlockSize() const {return 1;}

	//! use this if your input is in one piece and you don't want to call Update() and Final() separately
	virtual void CalculateDigest(byte *digest, const byte *input, unsigned int length)
		{Update(input, length); Final(digest);}

	//! verify that digest is a valid digest for the current message, then reinitialize the object
	/*! Default implementation is to call Final() and do a bitwise comparison
		between its output and digest. */
	virtual bool Verify(const byte *digest)
		{return TruncatedVerify(digest, DigestSize());}

	//! use this if your input is in one piece and you don't want to call Update() and Verify() separately
	virtual bool VerifyDigest(const byte *digest, const byte *input, unsigned int length)
		{Update(input, length); return Verify(digest);}

	//! truncated version of Final()
	virtual void TruncatedFinal(byte *digest, unsigned int digestSize) =0;

	//! truncated version of CalculateDigest()
	virtual void CalculateTruncatedDigest(byte *digest, unsigned int digestSize, const byte *input, unsigned int length)
		{Update(input, length); TruncatedFinal(digest, digestSize);}

	//! truncated version of Verify()
	virtual bool TruncatedVerify(const byte *digest, unsigned int digestLength);

	//! truncated version of VerifyDigest()
	virtual bool VerifyTruncatedDigest(const byte *digest, unsigned int digestLength, const byte *input, unsigned int length)
		{Update(input, length); return TruncatedVerify(digest, digestLength);}

protected:
	void ThrowIfInvalidTruncatedSize(unsigned int size) const;
};

//! .
template <class T>
class CRYPTOPP_DLL SimpleKeyedTransformation : public T, public SimpleKeyingInterface
{
public:
	void ThrowIfInvalidKeyLength(unsigned int length)
		{SimpleKeyingInterface::ThrowIfInvalidKeyLength(*this, length);}
};

//! .
typedef HashTransformation HashFunction;
#ifdef CRYPTOPP_DOXYGEN_PROCESSING
//! These objects usually should not be used directly. See BlockTransformation for more details.
class BlockCipher : public BlockTransformation, public SimpleKeyingInterface {};
//! interface for stream ciphers
class SymmetricCipher : public StreamTransformation, public SimpleKeyingInterface {};
//! interface for message authentication codes
class MessageAuthenticationCode : public HashTransformation, public SimpleKeyingInterface {};
#else
typedef SimpleKeyedTransformation<BlockTransformation> BlockCipher;
typedef SimpleKeyedTransformation<StreamTransformation> SymmetricCipher;
typedef SimpleKeyedTransformation<HashTransformation> MessageAuthenticationCode;

CRYPTOPP_DLL_TEMPLATE_CLASS SimpleKeyedTransformation<BlockTransformation>;
CRYPTOPP_DLL_TEMPLATE_CLASS SimpleKeyedTransformation<StreamTransformation>;
CRYPTOPP_DLL_TEMPLATE_CLASS SimpleKeyedTransformation<HashTransformation>;
#endif

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
typedef SymmetricCipher StreamCipher;
#endif

//! interface for random number generators
/*! All return values are uniformly distributed over the range specified.
*/
class CRYPTOPP_DLL RandomNumberGenerator : public Algorithm
{
public:
	//! generate new random byte and return it
	virtual byte GenerateByte() =0;

	//! generate new random bit and return it
	/*! Default implementation is to call GenerateByte() and return its parity. */
	virtual unsigned int GenerateBit();

	//! generate a random 32 bit word in the range min to max, inclusive
	virtual word32 GenerateWord32(word32 a=0, word32 b=0xffffffffL);

	//! generate random array of bytes
	/*! Default implementation is to call GenerateByte() size times. */
	virtual void GenerateBlock(byte *output, unsigned int size);

	//! generate and discard n bytes
	/*! Default implementation is to call GenerateByte() n times. */
	virtual void DiscardBytes(unsigned int n);

	//! randomly shuffle the specified array, resulting permutation is uniformly distributed
	template <class IT> void Shuffle(IT begin, IT end)
	{
		for (; begin != end; ++begin)
			std::iter_swap(begin, begin + GenerateWord32(0, end-begin-1));
	}

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
	byte GetByte() {return GenerateByte();}
	unsigned int GetBit() {return GenerateBit();}
	word32 GetLong(word32 a=0, word32 b=0xffffffffL) {return GenerateWord32(a, b);}
	word16 GetShort(word16 a=0, word16 b=0xffff) {return (word16)GenerateWord32(a, b);}
	void GetBlock(byte *output, unsigned int size) {GenerateBlock(output, size);}
#endif
};

//! returns a reference that can be passed to functions that ask for a RNG but doesn't actually use it
CRYPTOPP_DLL RandomNumberGenerator & NullRNG();

class WaitObjectContainer;

//! interface for objects that you can wait for

class Waitable
{
public:
	//! maximum number of wait objects that this object can return
	virtual unsigned int GetMaxWaitObjectCount() const =0;
	//! put wait objects into container
	virtual void GetWaitObjects(WaitObjectContainer &container) =0;
	//! wait on this object
	/*! same as creating an empty container, calling GetWaitObjects(), and calling Wait() on the container */
	bool Wait(unsigned long milliseconds);
};

//! interface for buffered transformations

/*! BufferedTransformation is a generalization of BlockTransformation,
	StreamTransformation, and HashTransformation.

	A buffered transformation is an object that takes a stream of bytes
	as input (this may be done in stages), does some computation on them, and
	then places the result into an internal buffer for later retrieval.  Any
	partial result already in the output buffer is not modified by further
	input.

	If a method takes a "blocking" parameter, and you
	pass "false" for it, the method will return before all input has been processed if
	the input cannot be processed without waiting (for network buffers to become available, for example).
	In this case the method will return true
	or a non-zero integer value. When this happens you must continue to call the method with the same
	parameters until it returns false or zero, before calling any other method on it or
	attached BufferedTransformation. The integer return value in this case is approximately
	the number of bytes left to be processed, and can be used to implement a progress bar.

	For functions that take a "propagation" parameter, propagation != 0 means pass on the signal to attached
	BufferedTransformation objects, with propagation decremented at each step until it reaches 0.
	-1 means unlimited propagation.

	\nosubgrouping
*/
class CRYPTOPP_DLL BufferedTransformation : public Algorithm, public Waitable
{
public:
	// placed up here for CW8
	static const std::string NULL_CHANNEL;	// the empty string ""

	BufferedTransformation() : Algorithm(false) {}

	//! return a reference to this object
	/*! This function is useful for passing a temporary BufferedTransformation object to a 
		function that takes a non-const reference. */
	BufferedTransformation& Ref() {return *this;}

	//!	\name INPUT
	//@{
		//! input a byte for processing
		unsigned int Put(byte inByte, bool blocking=true)
			{return Put(&inByte, 1, blocking);}
		//! input multiple bytes
		unsigned int Put(const byte *inString, unsigned int length, bool blocking=true)
			{return Put2(inString, length, 0, blocking);}

		//! input a 16-bit word
		unsigned int PutWord16(word16 value, ByteOrder order=BIG_ENDIAN_ORDER, bool blocking=true);
		//! input a 32-bit word
		unsigned int PutWord32(word32 value, ByteOrder order=BIG_ENDIAN_ORDER, bool blocking=true);

		//! request space to write bytes into for processing
		/*! \param size is requested size (as a hint) for input, and size of the returned space for output */
		virtual byte * CreatePutSpace(unsigned int &size) {size=0; return NULL;}

		virtual bool CanModifyInput() const {return false;}

		//! input multiple bytes that may be modified by callee
		unsigned int PutModifiable(byte *inString, unsigned int length, bool blocking=true)
			{return PutModifiable2(inString, length, 0, blocking);}

		bool MessageEnd(int propagation=-1, bool blocking=true)
			{return !!Put2(NULL, 0, propagation < 0 ? -1 : propagation+1, blocking);}
		unsigned int PutMessageEnd(const byte *inString, unsigned int length, int propagation=-1, bool blocking=true)
			{return Put2(inString, length, propagation < 0 ? -1 : propagation+1, blocking);}

		//! input multiple bytes for blocking or non-blocking processing
		/*! \param messageEnd means how many filters to signal MessageEnd to, including this one */
		virtual unsigned int Put2(const byte *inString, unsigned int length, int messageEnd, bool blocking) =0;
		//! input multiple bytes that may be modified by callee for blocking or non-blocking processing
		/*! \param messageEnd means how many filters to signal MessageEnd to, including this one */
		virtual unsigned int PutModifiable2(byte *inString, unsigned int length, int messageEnd, bool blocking)
			{return Put2(inString, length, messageEnd, blocking);}

		//! thrown by objects that have not implemented nonblocking input processing
		struct BlockingInputOnly : public NotImplemented
			{BlockingInputOnly(const std::string &s) : NotImplemented(s + ": Nonblocking input is not implemented by this object.") {}};
	//@}

	//!	\name WAITING
	//@{
		unsigned int GetMaxWaitObjectCount() const;
		void GetWaitObjects(WaitObjectContainer &container);
	//@}

	//!	\name SIGNALS
	//@{
		virtual void IsolatedInitialize(const NameValuePairs &parameters) {throw NotImplemented("BufferedTransformation: this object can't be reinitialized");}
		virtual bool IsolatedFlush(bool hardFlush, bool blocking) =0;
		virtual bool IsolatedMessageSeriesEnd(bool blocking) {return false;}

		//! initialize or reinitialize this object
		virtual void Initialize(const NameValuePairs &parameters=g_nullNameValuePairs, int propagation=-1);
		//! flush buffered input and/or output
		/*! \param hardFlush is used to indicate whether all data should be flushed
			\note Hard flushes must be used with care. It means try to process and output everything, even if
			there may not be enough data to complete the action. For example, hard flushing a HexDecoder would
			cause an error if you do it after inputing an odd number of hex encoded characters.
			For some types of filters, for example ZlibDecompressor, hard flushes can only
			be done at "synchronization points". These synchronization points are positions in the data
			stream that are created by hard flushes on the corresponding reverse filters, in this
			example ZlibCompressor. This is useful when zlib compressed data is moved across a
			network in packets and compression state is preserved across packets, as in the ssh2 protocol.
		*/
		virtual bool Flush(bool hardFlush, int propagation=-1, bool blocking=true);
		//! mark end of a series of messages
		/*! There should be a MessageEnd immediately before MessageSeriesEnd. */
		virtual bool MessageSeriesEnd(int propagation=-1, bool blocking=true);

		//! set propagation of automatically generated and transfered signals
		/*! propagation == 0 means do not automaticly generate signals */
		virtual void SetAutoSignalPropagation(int propagation) {}

		//!
		virtual int GetAutoSignalPropagation() const {return 0;}
public:

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
		void Close() {MessageEnd();}
#endif
	//@}

	//!	\name RETRIEVAL OF ONE MESSAGE
	//@{
		//! returns number of bytes that is currently ready for retrieval
		/*! All retrieval functions return the actual number of bytes
			retrieved, which is the lesser of the request number and
			MaxRetrievable(). */
		virtual unsigned long MaxRetrievable() const;

		//! returns whether any bytes are currently ready for retrieval
		virtual bool AnyRetrievable() const;

		//! try to retrieve a single byte
		virtual unsigned int Get(byte &outByte);
		//! try to retrieve multiple bytes
		virtual unsigned int Get(byte *outString, unsigned int getMax);

		//! peek at the next byte without removing it from the output buffer
		virtual unsigned int Peek(byte &outByte) const;
		//! peek at multiple bytes without removing them from the output buffer
		virtual unsigned int Peek(byte *outString, unsigned int peekMax) const;

		//! try to retrieve a 16-bit word
		unsigned int GetWord16(word16 &value, ByteOrder order=BIG_ENDIAN_ORDER);
		//! try to retrieve a 32-bit word
		unsigned int GetWord32(word32 &value, ByteOrder order=BIG_ENDIAN_ORDER);

		//! try to peek at a 16-bit word
		unsigned int PeekWord16(word16 &value, ByteOrder order=BIG_ENDIAN_ORDER);
		//! try to peek at a 32-bit word
		unsigned int PeekWord32(word32 &value, ByteOrder order=BIG_ENDIAN_ORDER);

		//! move transferMax bytes of the buffered output to target as input
		unsigned long TransferTo(BufferedTransformation &target, unsigned long transferMax=ULONG_MAX, const std::string &channel=NULL_CHANNEL)
			{TransferTo2(target, transferMax, channel); return transferMax;}

		//! discard skipMax bytes from the output buffer
		virtual unsigned long Skip(unsigned long skipMax=ULONG_MAX);

		//! copy copyMax bytes of the buffered output to target as input
		unsigned long CopyTo(BufferedTransformation &target, unsigned long copyMax=ULONG_MAX, const std::string &channel=NULL_CHANNEL) const
			{return CopyRangeTo(target, 0, copyMax, channel);}

		//! copy copyMax bytes of the buffered output, starting at position (relative to current position), to target as input
		unsigned long CopyRangeTo(BufferedTransformation &target, unsigned long position, unsigned long copyMax=ULONG_MAX, const std::string &channel=NULL_CHANNEL) const
			{unsigned long i = position; CopyRangeTo2(target, i, i+copyMax, channel); return i-position;}

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
		unsigned long MaxRetrieveable() const {return MaxRetrievable();}
#endif
	//@}

	//!	\name RETRIEVAL OF MULTIPLE MESSAGES
	//@{
		//!
		virtual unsigned long TotalBytesRetrievable() const;
		//! number of times MessageEnd() has been received minus messages retrieved or skipped
		virtual unsigned int NumberOfMessages() const;
		//! returns true if NumberOfMessages() > 0
		virtual bool AnyMessages() const;
		//! start retrieving the next message
		/*!
			Returns false if no more messages exist or this message 
			is not completely retrieved.
		*/
		virtual bool GetNextMessage();
		//! skip count number of messages
		virtual unsigned int SkipMessages(unsigned int count=UINT_MAX);
		//!
		unsigned int TransferMessagesTo(BufferedTransformation &target, unsigned int count=UINT_MAX, const std::string &channel=NULL_CHANNEL)
			{TransferMessagesTo2(target, count, channel); return count;}
		//!
		unsigned int CopyMessagesTo(BufferedTransformation &target, unsigned int count=UINT_MAX, const std::string &channel=NULL_CHANNEL) const;

		//!
		virtual void SkipAll();
		//!
		void TransferAllTo(BufferedTransformation &target, const std::string &channel=NULL_CHANNEL)
			{TransferAllTo2(target, channel);}
		//!
		void CopyAllTo(BufferedTransformation &target, const std::string &channel=NULL_CHANNEL) const;

		virtual bool GetNextMessageSeries() {return false;}
		virtual unsigned int NumberOfMessagesInThisSeries() const {return NumberOfMessages();}
		virtual unsigned int NumberOfMessageSeries() const {return 0;}
	//@}

	//!	\name NON-BLOCKING TRANSFER OF OUTPUT
	//@{
		//! .
		virtual unsigned int TransferTo2(BufferedTransformation &target, unsigned long &byteCount, const std::string &channel=NULL_CHANNEL, bool blocking=true) =0;
		virtual unsigned int CopyRangeTo2(BufferedTransformation &target, unsigned long &begin, unsigned long end=ULONG_MAX, const std::string &channel=NULL_CHANNEL, bool blocking=true) const =0;
		unsigned int TransferMessagesTo2(BufferedTransformation &target, unsigned int &messageCount, const std::string &channel=NULL_CHANNEL, bool blocking=true);
		unsigned int TransferAllTo2(BufferedTransformation &target, const std::string &channel=NULL_CHANNEL, bool blocking=true);
	//@}

	//!	\name CHANNELS
	//@{
		struct NoChannelSupport : public NotImplemented
			{NoChannelSupport() : NotImplemented("BufferedTransformation: this object doesn't support multiple channels") {}};

		unsigned int ChannelPut(const std::string &channel, byte inByte, bool blocking=true)
			{return ChannelPut(channel, &inByte, 1, blocking);}
		unsigned int ChannelPut(const std::string &channel, const byte *inString, unsigned int length, bool blocking=true)
			{return ChannelPut2(channel, inString, length, 0, blocking);}

		unsigned int ChannelPutModifiable(const std::string &channel, byte *inString, unsigned int length, bool blocking=true)
			{return ChannelPutModifiable2(channel, inString, length, 0, blocking);}

		unsigned int ChannelPutWord16(const std::string &channel, word16 value, ByteOrder order=BIG_ENDIAN_ORDER, bool blocking=true);
		unsigned int ChannelPutWord32(const std::string &channel, word32 value, ByteOrder order=BIG_ENDIAN_ORDER, bool blocking=true);

		bool ChannelMessageEnd(const std::string &channel, int propagation=-1, bool blocking=true)
			{return !!ChannelPut2(channel, NULL, 0, propagation < 0 ? -1 : propagation+1, blocking);}
		unsigned int ChannelPutMessageEnd(const std::string &channel, const byte *inString, unsigned int length, int propagation=-1, bool blocking=true)
			{return ChannelPut2(channel, inString, length, propagation < 0 ? -1 : propagation+1, blocking);}

		virtual byte * ChannelCreatePutSpace(const std::string &channel, unsigned int &size);

		virtual unsigned int ChannelPut2(const std::string &channel, const byte *begin, unsigned int length, int messageEnd, bool blocking);
		virtual unsigned int ChannelPutModifiable2(const std::string &channel, byte *begin, unsigned int length, int messageEnd, bool blocking);

		virtual void ChannelInitialize(const std::string &channel, const NameValuePairs &parameters=g_nullNameValuePairs, int propagation=-1);
		virtual bool ChannelFlush(const std::string &channel, bool hardFlush, int propagation=-1, bool blocking=true);
		virtual bool ChannelMessageSeriesEnd(const std::string &channel, int propagation=-1, bool blocking=true);

		virtual void SetRetrievalChannel(const std::string &channel);
	//@}

	//!	\name ATTACHMENT
	/*! Some BufferedTransformation objects (e.g. Filter objects)
		allow other BufferedTransformation objects to be attached. When
		this is done, the first object instead of buffering its output,
		sents that output to the attached object as input. The entire
		attachment chain is deleted when the anchor object is destructed.
	*/
	//@{
		//! returns whether this object allows attachment
		virtual bool Attachable() {return false;}
		//! returns the object immediately attached to this object or NULL for no attachment
		virtual BufferedTransformation *AttachedTransformation() {assert(!Attachable()); return 0;}
		//!
		virtual const BufferedTransformation *AttachedTransformation() const
			{return const_cast<BufferedTransformation *>(this)->AttachedTransformation();}
		//! delete the current attachment chain and replace it with newAttachment
		virtual void Detach(BufferedTransformation *newAttachment = 0)
			{assert(!Attachable()); throw NotImplemented("BufferedTransformation: this object is not attachable");}
		//! add newAttachment to the end of attachment chain
		virtual void Attach(BufferedTransformation *newAttachment);
	//@}

protected:
	static int DecrementPropagation(int propagation)
		{return propagation != 0 ? propagation - 1 : 0;}
};

//! returns a reference to a BufferedTransformation object that discards all input
BufferedTransformation & TheBitBucket();

//! interface for crypto material, such as public and private keys, and crypto parameters

class CRYPTOPP_DLL CryptoMaterial : public NameValuePairs
{
public:
	//! exception thrown when invalid crypto material is detected
	class CRYPTOPP_DLL InvalidMaterial : public InvalidDataFormat
	{
	public:
		explicit InvalidMaterial(const std::string &s) : InvalidDataFormat(s) {}
	};

	//! assign values from source to this object
	/*! \note This function can be used to create a public key from a private key. */
	virtual void AssignFrom(const NameValuePairs &source) =0;

	//! check this object for errors
	/*! \param level denotes the level of thoroughness:
		0 - using this object won't cause a crash or exception (rng is ignored)
		1 - this object will probably function (encrypt, sign, etc.) correctly (but may not check for weak keys and such)
		2 - make sure this object will function correctly, and do reasonable security checks
		3 - do checks that may take a long time
		\return true if the tests pass */
	virtual bool Validate(RandomNumberGenerator &rng, unsigned int level) const =0;

	//! throws InvalidMaterial if this object fails Validate() test
	virtual void ThrowIfInvalid(RandomNumberGenerator &rng, unsigned int level) const
		{if (!Validate(rng, level)) throw InvalidMaterial("CryptoMaterial: this object contains invalid values");}

//	virtual std::vector<std::string> GetSupportedFormats(bool includeSaveOnly=false, bool includeLoadOnly=false);

	//! save key into a BufferedTransformation
	virtual void Save(BufferedTransformation &bt) const
		{throw NotImplemented("CryptoMaterial: this object does not support saving");}

	//! load key from a BufferedTransformation
	/*! \throws KeyingErr if decode fails
		\note Generally does not check that the key is valid.
			Call ValidateKey() or ThrowIfInvalidKey() to check that. */
	virtual void Load(BufferedTransformation &bt)
		{throw NotImplemented("CryptoMaterial: this object does not support loading");}

	//! \return whether this object supports precomputation
	virtual bool SupportsPrecomputation() const {return false;}
	//! do precomputation
	/*! The exact semantics of Precompute() is varies, but
		typically it means calculate a table of n objects
		that can be used later to speed up computation. */
	virtual void Precompute(unsigned int n)
		{assert(!SupportsPrecomputation()); throw NotImplemented("CryptoMaterial: this object does not support precomputation");}
	//! retrieve previously saved precomputation
	virtual void LoadPrecomputation(BufferedTransformation &storedPrecomputation)
		{assert(!SupportsPrecomputation()); throw NotImplemented("CryptoMaterial: this object does not support precomputation");}
	//! save precomputation for later use
	virtual void SavePrecomputation(BufferedTransformation &storedPrecomputation) const
		{assert(!SupportsPrecomputation()); throw NotImplemented("CryptoMaterial: this object does not support precomputation");}

	// for internal library use
	void DoQuickSanityCheck() const	{ThrowIfInvalid(NullRNG(), 0);}
};

//! interface for generatable crypto material, such as private keys and crypto parameters

class CRYPTOPP_DLL GeneratableCryptoMaterial : virtual public CryptoMaterial
{
public:
	//! generate a random key or crypto parameters
	/*! \throws KeyingErr if algorithm parameters are invalid, or if a key can't be generated
		(e.g., if this is a public key object) */
	virtual void GenerateRandom(RandomNumberGenerator &rng, const NameValuePairs &params = g_nullNameValuePairs)
		{throw NotImplemented("GeneratableCryptoMaterial: this object does not support key/parameter generation");}

	//! calls the above function with a NameValuePairs object that just specifies "KeySize"
	void GenerateRandomWithKeySize(RandomNumberGenerator &rng, unsigned int keySize);
};

//! interface for public keys

class CRYPTOPP_DLL PublicKey : virtual public CryptoMaterial
{
};

//! interface for private keys

class CRYPTOPP_DLL PrivateKey : public GeneratableCryptoMaterial
{
};

//! interface for crypto prameters

class CRYPTOPP_DLL CryptoParameters : public GeneratableCryptoMaterial
{
};

//! interface for asymmetric algorithms

class CRYPTOPP_DLL AsymmetricAlgorithm : public Algorithm
{
public:
	//! returns a reference to the crypto material used by this object
	virtual CryptoMaterial & AccessMaterial() =0;
	//! returns a const reference to the crypto material used by this object
	virtual const CryptoMaterial & GetMaterial() const =0;

	//! for backwards compatibility, calls AccessMaterial().Load(bt)
	void BERDecode(BufferedTransformation &bt)
		{AccessMaterial().Load(bt);}
	//! for backwards compatibility, calls GetMaterial().Save(bt)
	void DEREncode(BufferedTransformation &bt) const
		{GetMaterial().Save(bt);}
};

//! interface for asymmetric algorithms using public keys

class CRYPTOPP_DLL PublicKeyAlgorithm : public AsymmetricAlgorithm
{
public:
	// VC60 workaround: no co-variant return type
	CryptoMaterial & AccessMaterial() {return AccessPublicKey();}
	const CryptoMaterial & GetMaterial() const {return GetPublicKey();}

	virtual PublicKey & AccessPublicKey() =0;
	virtual const PublicKey & GetPublicKey() const {return const_cast<PublicKeyAlgorithm *>(this)->AccessPublicKey();}
};

//! interface for asymmetric algorithms using private keys

class CRYPTOPP_DLL PrivateKeyAlgorithm : public AsymmetricAlgorithm
{
public:
	CryptoMaterial & AccessMaterial() {return AccessPrivateKey();}
	const CryptoMaterial & GetMaterial() const {return GetPrivateKey();}

	virtual PrivateKey & AccessPrivateKey() =0;
	virtual const PrivateKey & GetPrivateKey() const {return const_cast<PrivateKeyAlgorithm *>(this)->AccessPrivateKey();}
};

//! interface for key agreement algorithms

class CRYPTOPP_DLL KeyAgreementAlgorithm : public AsymmetricAlgorithm
{
public:
	CryptoMaterial & AccessMaterial() {return AccessCryptoParameters();}
	const CryptoMaterial & GetMaterial() const {return GetCryptoParameters();}

	virtual CryptoParameters & AccessCryptoParameters() =0;
	virtual const CryptoParameters & GetCryptoParameters() const {return const_cast<KeyAgreementAlgorithm *>(this)->AccessCryptoParameters();}
};

//! interface for public-key encryptors and decryptors

/*! This class provides an interface common to encryptors and decryptors
	for querying their plaintext and ciphertext lengths.
*/
class CRYPTOPP_DLL PK_CryptoSystem
{
public:
	virtual ~PK_CryptoSystem() {}

	//! maximum length of plaintext for a given ciphertext length
	/*! \note This function returns 0 if cipherTextLength is not valid (too long or too short). */
	virtual unsigned int MaxPlaintextLength(unsigned int cipherTextLength) const =0;

	//! calculate length of ciphertext given length of plaintext
	/*! \note This function returns 0 if plainTextLength is not valid (too long). */
	virtual unsigned int CiphertextLength(unsigned int plainTextLength) const =0;

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
	unsigned int MaxPlainTextLength(unsigned int cipherTextLength) const {return MaxPlaintextLength(cipherTextLength);}
	unsigned int CipherTextLength(unsigned int plainTextLength) const {return CiphertextLength(plainTextLength);}
#endif
};

//! interface for public-key encryptors

class CRYPTOPP_DLL PK_Encryptor : virtual public PK_CryptoSystem, public PublicKeyAlgorithm
{
public:
	//! .
	class CRYPTOPP_DLL InvalidPlaintextLength : public Exception
	{
	public:
		InvalidPlaintextLength() : Exception(OTHER_ERROR, "PK_Encryptor: invalid plaintext length") {}
	};

	//! encrypt a byte string
	/*! \pre CipherTextLength(plainTextLength) != 0 (i.e., plainText isn't too long)
		\pre size of cipherText == CipherTextLength(plainTextLength)
	*/
	virtual void Encrypt(RandomNumberGenerator &rng, const byte *plainText, unsigned int plainTextLength, byte *cipherText) const =0;

	//! create a new encryption filter
	/*! \note caller is responsible for deleting the returned pointer
	*/
	virtual BufferedTransformation * CreateEncryptionFilter(RandomNumberGenerator &rng, BufferedTransformation *attachment=NULL) const;
};

//! interface for public-key decryptors

class CRYPTOPP_DLL PK_Decryptor : virtual public PK_CryptoSystem, public PrivateKeyAlgorithm
{
public:
	//! decrypt a byte string, and return the length of plaintext
	/*! \pre size of plainText == MaxPlainTextLength(cipherTextLength) bytes.
		\return the actual length of the plaintext, or 0 if decryption fails.
	*/
	virtual DecodingResult Decrypt(const byte *cipherText, unsigned int cipherTextLength, byte *plainText) const =0;

	//! create a new decryption filter
	/*! \note caller is responsible for deleting the returned pointer
	*/
	virtual BufferedTransformation * CreateDecryptionFilter(BufferedTransformation *attachment=NULL) const;
};

//! interface for encryptors and decryptors with fixed length ciphertext

/*! A simplified interface is provided for crypto systems (such
	as RSA) whose ciphertext length and maximum plaintext length
	depend only on the key.
*/
class CRYPTOPP_DLL PK_FixedLengthCryptoSystem : virtual public PK_CryptoSystem
{
public:
	//!
	virtual unsigned int FixedMaxPlaintextLength() const =0;
	//!
	virtual unsigned int FixedCiphertextLength() const =0;

	unsigned int MaxPlaintextLength(unsigned int cipherTextLength) const;
	unsigned int CiphertextLength(unsigned int plainTextLength) const;
	
#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
	unsigned int MaxPlainTextLength(unsigned int cipherTextLength) const {return MaxPlaintextLength(cipherTextLength);}
	unsigned int CipherTextLength(unsigned int plainTextLength) const {return CiphertextLength(plainTextLength);}
	unsigned int MaxPlainTextLength() const {return FixedMaxPlaintextLength();}
	unsigned int CipherTextLength() const {return FixedCiphertextLength();}
#endif
};

//! interface for encryptors with fixed length ciphertext

class CRYPTOPP_DLL PK_FixedLengthEncryptor : public PK_Encryptor, virtual public PK_FixedLengthCryptoSystem
{
};

//! interface for decryptors with fixed length ciphertext

class CRYPTOPP_DLL PK_FixedLengthDecryptor : public PK_Decryptor, virtual public PK_FixedLengthCryptoSystem
{
public:
	//! decrypt a byte string, and return the length of plaintext
	/*! \pre length of cipherText == CipherTextLength()
		\pre size of plainText == MaxPlainTextLength()
		\return the actual length of the plaintext, or 0 if decryption fails.
	*/
	virtual DecodingResult FixedLengthDecrypt(const byte *cipherText, byte *plainText) const =0;

	DecodingResult Decrypt(const byte *cipherText, unsigned int cipherTextLength, byte *plainText) const;

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
	DecodingResult Decrypt(const byte *cipherText, byte *plainText) const {return FixedLengthDecrypt(cipherText, plainText);}
#endif
};

//! interface for public-key signers and verifiers

/*! This class provides an interface common to signers and verifiers
	for querying their signature lengths and creating message
	accumulators.
*/
class CRYPTOPP_DLL PK_SignatureScheme
{
public:
	virtual ~PK_SignatureScheme() {}

	//! signature length support by this object (as either input or output)
	virtual unsigned int SignatureLength() const =0;

	//! deprecated, please use PK_Signer::NewSignatureAccumulator or PK_Verifier::NewVerificationAccumulator instead
	virtual HashTransformation * NewMessageAccumulator() const =0;
};

//! interface for public-key signers

class CRYPTOPP_DLL PK_Signer : virtual public PK_SignatureScheme, public PrivateKeyAlgorithm
{
public:
	//! key too short exception, may be thrown by Sign() or SignMessage()
	class CRYPTOPP_DLL KeyTooShort : public Exception
	{
	public:
		KeyTooShort() : Exception(OTHER_ERROR, "PK_Signer: key too short") {}
	};

	//! sign and delete messageAccumulator (even in case of exception thrown)
	/*! \pre messageAccumulator was obtained by calling NewSignatureAccumulator()
		\pre HashTransformation::Final() has not been called on messageAccumulator
		\pre size of signature == SignatureLength()
	*/
	virtual void Sign(RandomNumberGenerator &rng, HashTransformation *messageAccumulator, byte *signature) const;

	//! sign and restart messageAccumulator
	virtual void SignAndRestart(RandomNumberGenerator &rng, HashTransformation &messageAccumulator, byte *signature) const =0;

	//! sign a message
	/*! \pre size of signature == SignatureLength() */
	virtual void SignMessage(RandomNumberGenerator &rng, const byte *message, unsigned int messageLen, byte *signature) const;

	//! create a new HashTransformation to accumulate the message to be signed
	virtual HashTransformation * NewSignatureAccumulator() const
		{return NewMessageAccumulator();}
};

//! interface for public-key signature verifiers

class CRYPTOPP_DLL PK_Verifier : virtual public PK_SignatureScheme, public PublicKeyAlgorithm
{
public:
	/*! If this function returns true, you must input the signature when
		calling NewVerificationAccumulator(). Otherwise, you must input the signature
		when calling Verify(). */
	virtual bool SignatureUpfrontForVerification() const {return false;}

	//! create a new HashTransformation to accumulate the message to be verified
	/*! \param signature is ignored if SignatureUpfrontForVerification() == false
		\param signature may be NULL to indicate that the signature is not available yet
	*/
	virtual HashTransformation * NewVerificationAccumulator(const byte *signature=NULL) const
		{return NewMessageAccumulator();}

	//! check whether sig is a valid signature for messageAccumulator, and delete messageAccumulator (even in case of exception thrown)
	/*! \pre messageAccumulator was obtained by calling NewVerificationAccumulator()
		\pre HashTransformation::Final() has not been called on messageAccumulator
		\pre length of signature == SignatureLength()
		\param signature is ignored if SignatureUpfrontForVerification() == true
	*/
	virtual bool Verify(HashTransformation *messageAccumulator, const byte *signature=NULL) const;

	//! check whether sig is a valid signature for messageAccumulator, and restart messageAccumulator
	/*! \note depending on SignatureUpfrontForVerification(), signature is either the current or the next signature
		\param signature may be NULL to indicate that the next signature is not available yet
	*/
	virtual bool VerifyAndRestart(HashTransformation &messageAccumulator, const byte *signature) const =0;

	//! only useful if SignatureUpfrontForVerification() == true
	virtual void InitializeVerificationAccumulator(HashTransformation &messageAccumulator, const byte *signature) const {}

	//! check whether sig is a valid signature for message
	/*! \pre size of signature == SignatureLength() */
	virtual bool VerifyMessage(const byte *message, unsigned int messageLen, const byte *signature) const;
};

//! interface for public-key signers and verifiers with recovery

/*! In a signature scheme with recovery, a verifier is able to extract
	a message from its valid signature.
*/
class CRYPTOPP_DLL PK_SignatureSchemeWithRecovery : virtual public PK_SignatureScheme
{
public:
	//! length of longest message that can be fully recovered
	virtual unsigned int MaximumRecoverableLength() const =0;

	//! whether or not messages longer than MaximumRecoverableLength() can be signed
	/*! If this function returns false, any message longer than
		MaximumRecoverableLength() will be truncated for signature
		and will fail verification.
	*/
	virtual bool AllowLeftoverMessage() const =0;
};

//! interface for public-key signers with recovery

class CRYPTOPP_DLL PK_SignerWithRecovery : virtual public PK_SignatureSchemeWithRecovery, virtual public PK_Signer
{
};

//! interface for public-key verifiers with recovery

/*! A PK_VerifierWithRecovery can also be used the same way as a PK_Verifier,
	where the signature and the entire message is given to Verify() or
	VerifyMessage() as input.
*/
class CRYPTOPP_DLL PK_VerifierWithRecovery : virtual public PK_SignatureSchemeWithRecovery, virtual public PK_Verifier
{
public:
	/*! If this function returns true, you must input the signature when
		calling NewRecoveryAccumulator(). Otherwise, you must input the signature
		when calling Recover(). */
	virtual bool SignatureUpfrontForRecovery() const =0;

	//! create a new HashTransformation to accumulate leftover message
	virtual HashTransformation * NewRecoveryAccumulator(const byte *signature=NULL) const =0;

	//! recover a message from its signature
	/*! \pre leftoverMessageAccumulator was obtained by calling NewLeftoverMessageAccumulator(signature)
		\pre HashTransformation::Final() has not been called on leftoverMessageAccumulator
		\pre length of signature == SignatureLength()
		\pre size of recoveredMessage == MaximumRecoverableLength()
	*/
	virtual DecodingResult Recover(byte *recoveredMessage, HashTransformation *recoveryAccumulator, const byte *signature=NULL) const =0;

	//! recover a message from its signature
	/*! depending on SignatureUpfrontForRecovery(), signature is either the current or the next signature */
	// TODO: uncomment this and implement
	// virtual unsigned int RecoverAndRestart(byte *recoveredMessage, HashTransformation &recoveryAccumulator, const byte *signature) const =0;

	//! recover a message from its signature
	/*! \note This function should be equivalent to Recover(recoveredMessage, NewRecoveryAccumulator(signature), signature)
		\pre length of signature == SignatureLength()
		\pre size of recoveredMessage == MaximumRecoverableLength()
	*/
	virtual DecodingResult RecoverMessage(byte *recoveredMessage, const byte *message, unsigned int messageLen, const byte *signature) const
		{return Recover(recoveredMessage, NewRecoveryAccumulator(signature), signature);}
};

//! interface for domains of simple key agreement protocols

/*! A key agreement domain is a set of parameters that must be shared
	by two parties in a key agreement protocol, along with the algorithms
	for generating key pairs and deriving agreed values.
*/
class CRYPTOPP_DLL SimpleKeyAgreementDomain : public KeyAgreementAlgorithm
{
public:
	//! return length of agreed value produced
	virtual unsigned int AgreedValueLength() const =0;
	//! return length of private keys in this domain
	virtual unsigned int PrivateKeyLength() const =0;
	//! return length of public keys in this domain
	virtual unsigned int PublicKeyLength() const =0;
	//! generate private key
	/*! \pre size of privateKey == PrivateKeyLength() */
	virtual void GeneratePrivateKey(RandomNumberGenerator &rng, byte *privateKey) const =0;
	//! generate public key
	/*!	\pre size of publicKey == PublicKeyLength() */
	virtual void GeneratePublicKey(RandomNumberGenerator &rng, const byte *privateKey, byte *publicKey) const =0;
	//! generate private/public key pair
	/*! \note equivalent to calling GeneratePrivateKey() and then GeneratePublicKey() */
	virtual void GenerateKeyPair(RandomNumberGenerator &rng, byte *privateKey, byte *publicKey) const;
	//! derive agreed value from your private key and couterparty's public key, return false in case of failure
	/*! \note If you have previously validated the public key, use validateOtherPublicKey=false to save time.
	/*! \pre size of agreedValue == AgreedValueLength()
		\pre length of privateKey == PrivateKeyLength()
		\pre length of otherPublicKey == PublicKeyLength()
	*/
	virtual bool Agree(byte *agreedValue, const byte *privateKey, const byte *otherPublicKey, bool validateOtherPublicKey=true) const =0;

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
	bool ValidateDomainParameters(RandomNumberGenerator &rng) const
		{return GetCryptoParameters().Validate(rng, 2);}
#endif
};

//! interface for domains of authenticated key agreement protocols

/*! In an authenticated key agreement protocol, each party has two
	key pairs. The long-lived key pair is called the static key pair,
	and the short-lived key pair is called the ephemeral key pair.
*/
class CRYPTOPP_DLL AuthenticatedKeyAgreementDomain : public KeyAgreementAlgorithm
{
public:
	//! return length of agreed value produced
	virtual unsigned int AgreedValueLength() const =0;

	//! return length of static private keys in this domain
	virtual unsigned int StaticPrivateKeyLength() const =0;
	//! return length of static public keys in this domain
	virtual unsigned int StaticPublicKeyLength() const =0;
	//! generate static private key
	/*! \pre size of privateKey == PrivateStaticKeyLength() */
	virtual void GenerateStaticPrivateKey(RandomNumberGenerator &rng, byte *privateKey) const =0;
	//! generate static public key
	/*!	\pre size of publicKey == PublicStaticKeyLength() */
	virtual void GenerateStaticPublicKey(RandomNumberGenerator &rng, const byte *privateKey, byte *publicKey) const =0;
	//! generate private/public key pair
	/*! \note equivalent to calling GenerateStaticPrivateKey() and then GenerateStaticPublicKey() */
	virtual void GenerateStaticKeyPair(RandomNumberGenerator &rng, byte *privateKey, byte *publicKey) const;

	//! return length of ephemeral private keys in this domain
	virtual unsigned int EphemeralPrivateKeyLength() const =0;
	//! return length of ephemeral public keys in this domain
	virtual unsigned int EphemeralPublicKeyLength() const =0;
	//! generate ephemeral private key
	/*! \pre size of privateKey == PrivateEphemeralKeyLength() */
	virtual void GenerateEphemeralPrivateKey(RandomNumberGenerator &rng, byte *privateKey) const =0;
	//! generate ephemeral public key
	/*!	\pre size of publicKey == PublicEphemeralKeyLength() */
	virtual void GenerateEphemeralPublicKey(RandomNumberGenerator &rng, const byte *privateKey, byte *publicKey) const =0;
	//! generate private/public key pair
	/*! \note equivalent to calling GenerateEphemeralPrivateKey() and then GenerateEphemeralPublicKey() */
	virtual void GenerateEphemeralKeyPair(RandomNumberGenerator &rng, byte *privateKey, byte *publicKey) const;

	//! derive agreed value from your private keys and couterparty's public keys, return false in case of failure
	/*! \note The ephemeral public key will always be validated.
		      If you have previously validated the static public key, use validateStaticOtherPublicKey=false to save time.
		\pre size of agreedValue == AgreedValueLength()
		\pre length of staticPrivateKey == StaticPrivateKeyLength()
		\pre length of ephemeralPrivateKey == EphemeralPrivateKeyLength()
		\pre length of staticOtherPublicKey == StaticPublicKeyLength()
		\pre length of ephemeralOtherPublicKey == EphemeralPublicKeyLength()
	*/
	virtual bool Agree(byte *agreedValue,
		const byte *staticPrivateKey, const byte *ephemeralPrivateKey,
		const byte *staticOtherPublicKey, const byte *ephemeralOtherPublicKey,
		bool validateStaticOtherPublicKey=true) const =0;

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
	bool ValidateDomainParameters(RandomNumberGenerator &rng) const
		{return GetCryptoParameters().Validate(rng, 2);}
#endif
};

// interface for password authenticated key agreement protocols, not implemented yet
#if 0
//! interface for protocol sessions
/*! The methods should be called in the following order:

	InitializeSession(rng, parameters);	// or call initialize method in derived class
	while (true)
	{
		if (OutgoingMessageAvailable())
		{
			length = GetOutgoingMessageLength();
			GetOutgoingMessage(message);
			; // send outgoing message
		}

		if (LastMessageProcessed())
			break;

		; // receive incoming message
		ProcessIncomingMessage(message);
	}
	; // call methods in derived class to obtain result of protocol session
*/
class ProtocolSession
{
public:
	//! exception thrown when an invalid protocol message is processed
	class ProtocolError : public Exception
	{
	public:
		ProtocolError(ErrorType errorType, const std::string &s) : Exception(errorType, s) {}
	};

	//! exception thrown when a function is called unexpectedly
	/*! for example calling ProcessIncomingMessage() when ProcessedLastMessage() == true */
	class UnexpectedMethodCall : public Exception
	{
	public:
		UnexpectedMethodCall(const std::string &s) : Exception(OTHER_ERROR, s) {}
	};

	ProtocolSession() : m_rng(NULL), m_throwOnProtocolError(true), m_validState(false) {}
	virtual ~ProtocolSession() {}

	virtual void InitializeSession(RandomNumberGenerator &rng, const NameValuePairs &parameters) =0;

	bool GetThrowOnProtocolError() const {return m_throwOnProtocolError;}
	void SetThrowOnProtocolError(bool throwOnProtocolError) {m_throwOnProtocolError = throwOnProtocolError;}

	bool HasValidState() const {return m_validState;}

	virtual bool OutgoingMessageAvailable() const =0;
	virtual unsigned int GetOutgoingMessageLength() const =0;
	virtual void GetOutgoingMessage(byte *message) =0;

	virtual bool LastMessageProcessed() const =0;
	virtual void ProcessIncomingMessage(const byte *message, unsigned int messageLength) =0;

protected:
	void HandleProtocolError(Exception::ErrorType errorType, const std::string &s) const;
	void CheckAndHandleInvalidState() const;
	void SetValidState(bool valid) {m_validState = valid;}

	RandomNumberGenerator *m_rng;

private:
	bool m_throwOnProtocolError, m_validState;
};

class KeyAgreementSession : public ProtocolSession
{
public:
	virtual unsigned int GetAgreedValueLength() const =0;
	virtual void GetAgreedValue(byte *agreedValue) const =0;
};

class PasswordAuthenticatedKeyAgreementSession : public KeyAgreementSession
{
public:
	void InitializePasswordAuthenticatedKeyAgreementSession(RandomNumberGenerator &rng, 
		const byte *myId, unsigned int myIdLength, 
		const byte *counterPartyId, unsigned int counterPartyIdLength, 
		const byte *passwordOrVerifier, unsigned int passwordOrVerifierLength);
};

class PasswordAuthenticatedKeyAgreementDomain : public KeyAgreementAlgorithm
{
public:
	//! return whether the domain parameters stored in this object are valid
	virtual bool ValidateDomainParameters(RandomNumberGenerator &rng) const
		{return GetCryptoParameters().Validate(rng, 2);}

	virtual unsigned int GetPasswordVerifierLength(const byte *password, unsigned int passwordLength) const =0;
	virtual void GeneratePasswordVerifier(RandomNumberGenerator &rng, const byte *userId, unsigned int userIdLength, const byte *password, unsigned int passwordLength, byte *verifier) const =0;

	enum RoleFlags {CLIENT=1, SERVER=2, INITIATOR=4, RESPONDER=8};

	virtual bool IsValidRole(unsigned int role) =0;
	virtual PasswordAuthenticatedKeyAgreementSession * CreateProtocolSession(unsigned int role) const =0;
};
#endif

//! BER Decode Exception Class, may be thrown during an ASN1 BER decode operation
class CRYPTOPP_DLL BERDecodeErr : public InvalidArgument
{
public: 
	BERDecodeErr() : InvalidArgument("BER decode error") {}
	BERDecodeErr(const std::string &s) : InvalidArgument(s) {}
};

//! interface for encoding and decoding ASN1 objects
class CRYPTOPP_DLL ASN1Object
{
public:
	virtual ~ASN1Object() {}
	//! decode this object from a BufferedTransformation, using BER (Basic Encoding Rules)
	virtual void BERDecode(BufferedTransformation &bt) =0;
	//! encode this object into a BufferedTransformation, using DER (Distinguished Encoding Rules)
	virtual void DEREncode(BufferedTransformation &bt) const =0;
	//! encode this object into a BufferedTransformation, using BER
	/*! this may be useful if DEREncode() would be too inefficient */
	virtual void BEREncode(BufferedTransformation &bt) const {DEREncode(bt);}
};

#ifdef CRYPTOPP_MAINTAIN_BACKWARDS_COMPATIBILITY
typedef PK_SignatureScheme PK_SignatureSystem
typedef PK_SignatureSchemeWithRecovery PK_SignatureSystemWithRecovery
typedef SimpleKeyAgreementDomain PK_SimpleKeyAgreementDomain
typedef AuthenticatedKeyAgreementDomain PK_AuthenticatedKeyAgreementDomain
typedef WithPrecomputation PK_WithPrecomputation
#endif

NAMESPACE_END

#endif
