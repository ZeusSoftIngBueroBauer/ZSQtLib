@page _PAGE_Main_CodingGuidelines Coding Guidelines

For better readability naming conventions and coding guidelines are follewed.

User defined data types:

Start with an uppercase letter:

  E .. Enumerations (EResolution)
  C .. Classes (CEnumeration)
  S .. Structures (SEnumEntry)

Variablen:

- Start with a lowercase letter that indicates the data type (iEnumerator, ariVals, strlstVals, pCmb, pLbl, pWdgt).
- Instanz variables start with "m_" (m_idx, m_strName).
- Class variables start with "s_" (s_strlstNames).
- Constants start with "c_" (c_iPI).
- Method input arguments start with "i_" (i_strlstNames).
- Method output arguments start with "o_" (o_piErrNo).

Methods:

- Start with a lowercase letter.
  There are a view exceptions. E.g. if static class methods and instance methods have the same name
  class methods start with a uppercase letter and the instance methods with a lowercase letter
  (e.g. static QString NameSpace(), virtual QString nameSpace()).

- GetInstance / CreateInstance / ReleaseInstance

  - GetInstance always returns a reference to an existing instance or nullptr but
    will never create an instance if not yet existing.
  - CreateInstance always returns a reference to an instance.
    If the instance did not exist before a reference counter is incremented.
    ReleaseInstance must always be called after a CreateInstance call if the instance
    is no longer needed.
  - ReleaseInstance decrements a reference counter. If the reference counter becomes 0
    the instance is deleted.
