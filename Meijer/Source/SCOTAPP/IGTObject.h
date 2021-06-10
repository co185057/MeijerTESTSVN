#ifndef _IGTOBJECT_H
#define _IGTOBJECT_H


/**
 * \brief Interface for accessing configuration information.
 */
class IGTObject
{
public:
    IGTObject() {}
    virtual ~IGTObject() {}

	virtual _bstr_t XML() = 0;
	virtual _variant_t GetValue2(_bstr_t Name) = 0;
	virtual long GetListCount(_bstr_t Name) = 0;
	virtual unsigned char SetCurrentNode(_bstr_t path, long index) = 0;
	virtual _variant_t GetListValue2(_bstr_t Name, long index) = 0;

};

#endif // _IGTOBJECT_H
