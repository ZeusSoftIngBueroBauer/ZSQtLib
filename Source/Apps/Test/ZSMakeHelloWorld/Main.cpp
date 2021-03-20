/* In this exercise we want to design a simple binary search tree with different kinds of iterators.
 * Our binary tree should store an arbitrary type which is comparable (e.g. int, string, float etc.)
 * The tree should provide an "add(...)" method which inserts a new element
 * into the tree (preserving the binary search tree's invariant) as well as a "search(...)" method.
 * In this exercise we do not care about deleting elements in the search tree.
 * The tree should provide three different iterators which traverse the tree in preorder, postorder and inorder.
 *
 * E.g.:
 * Assume we have the following binary tree:
 *                               6
 *                          4         8
 *                       2     5   7      9
 *
 * Consider having the instance "bt" of our binary tree.
 * To construct the above tree and traverse it in preorder we do:
 *     bt.add(6);
 *     bt.add(4);
 *     bt.add(8);
 *     bt.add(2);
 *     bt.add(5);
 *     bt.add(7);
 *     bt.add(9);
 *     auto itPre = bt.beginPreorder();
 *     for(;itPre != bt.endPreorder(); ++itPre) {
 *        std::cout << *itPre << " ";
 *     }
 *
 * The output of this code snippet should be "6 4 2 5 8 7 9".
 * Similiarly we want iterators for postorder (beginPostorder() ; endPostOrder()) and for
 * inorder (beginInorder() ; endInorder()). If you're not familiar with the traversion orders you can 
 * look them up online.
 *
 * A very important part of this exercise are the unit tests. Show how you would test the functionality of the
 * binary search tree and the three different iterators.
 * Don't use any testing framework. Simple if-statements are sufficient for this exercise.
 *
 */

#include <iostream>
#include <iterator>


template <typename T> class CBSTree;

template <typename T>
struct SBSTreeNode
{
public:
    SBSTreeNode<T>() :
        m_val(),
        m_pUp(nullptr),
        m_pLeft(nullptr),
        m_pRight(nullptr)
    {
    }
    ~SBSTreeNode<T>()
    {
        m_pUp = nullptr;
        m_pLeft = nullptr;
        m_pRight = nullptr;
    }
public:
    SBSTreeNode& operator = ( const T& i_val )
    {
        m_val = i_val;
        return *this;
    }
    bool operator == ( SBSTreeNode& i_other )
    {
        bool bEqual = true;
        if( m_val != i_other.m_val ) {
            bEqual = false;
        } else if ( m_pUp != i_other.m_pUp ) {
            bEqual = false;
        } else if ( m_pLeft != i_other.m_pLeft ) {
            bEqual = false;
        } else if ( m_pRight != i_other.m_pRight ) {
            bEqual = false;
        }
        return bEqual;
    }
    bool operator != ( SBSTreeNode& i_other )
    {
        return !(*this == i_other);
    }
public:
    T            m_val;
    SBSTreeNode* m_pUp;
    SBSTreeNode* m_pLeft;
    SBSTreeNode* m_pRight;
};

template <typename T>
class CBSTree
{
public:
    CBSTree() :
        m_pBSTree(nullptr)
    {
    }
    ~CBSTree()
    {
        destroy(m_pBSTree);
    }
public:
    class iterator
    {
    friend class CBSTree<T>;
    enum ETraversalOrder {
        EPreOrder,
        EPostOrder,
        EInOrder
    };
    public:
        iterator() {}
        iterator( SBSTreeNode<T>* i_pNode ) : m_pNodeCurr(i_pNode) {}
    public:
        T operator * () const;
        bool operator == ( iterator& i_other );
        bool operator != ( iterator& i_other );
        iterator& operator ++ ();
    private:
        SBSTreeNode<T>* m_pRoot = nullptr;
        SBSTreeNode<T>* m_pNodeCurr = nullptr;
        ETraversalOrder m_travOrd = EPostOrder;
    #ifdef _DEBUG
    public:
        int m_iSearchSteps = 0;
    #endif
    };
public:
    void add( const T& i_val );
    void destroy( SBSTreeNode<T>* i_pNode ) { /* not necessary in this test*/ }
    iterator search( const T& i_val );
public:
    iterator beginPreorder();
    iterator beginPostorder();
    iterator beginInorder();
    iterator end();
protected:
    void traversePostorder( SBSTreeNode<T>*& i_pNode );
protected:
    void add( SBSTreeNode<T>*& i_pNode, SBSTreeNode<T>* i_pNodeUp, const T& i_val );
private:
    SBSTreeNode<T>* m_pBSTree;
};

template <typename T>
void CBSTree<T>::add( const T& i_val )
{
    add(m_pBSTree, nullptr, i_val);
}

template <typename T>
void CBSTree<T>::add( SBSTreeNode<T>*& i_pNode, SBSTreeNode<T>* i_pNodeUp, const T& i_val )
{
    if( i_pNode == nullptr )
    {
        i_pNode = new SBSTreeNode<T>();
        *i_pNode = i_val;
        i_pNode->m_pUp = i_pNodeUp;
    }
    else if( i_val == i_pNode->m_val )
    {
        throw std::exception("Value already existing");
    }
    else if( i_val < i_pNode->m_val )
    {
        add(i_pNode->m_pLeft, i_pNode, i_val);
    }
    else // if( i_val > i_pNode->m_val )
    {
        add(i_pNode->m_pRight, i_pNode, i_val);
    }
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::search( const T& i_val )
{
    SBSTreeNode<T>* pNode = m_pBSTree;
    #ifdef _DEBUG
    auto iSearchSteps = 0;
    #endif
    while( pNode != nullptr )
    {
        if( i_val == pNode->m_val )
        {
            break;
        }
        else if( i_val < pNode->m_val )
        {
            pNode = pNode->m_pLeft;
        }
        else // if( i_val > pNode->m_val )
        {
            pNode = pNode->m_pRight;
        }
        #ifdef _DEBUG
        ++iSearchSteps;
        #endif
    }
    CBSTree<T>::iterator it(pNode);
    #ifdef _DEBUG
    it.m_iSearchSteps = iSearchSteps;
    #endif
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::beginPreorder()
{
    CBSTree<T>::iterator it(m_pBSTree);
    it.m_pRoot = m_pBSTree;
    it.m_travOrd = CBSTree<T>::iterator::EPreOrder;
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::beginPostorder()
{
    SBSTreeNode<T>* pNode = m_pBSTree;
    traversePostorder(pNode);
    CBSTree<T>::iterator it(pNode);
    it.m_pRoot = m_pBSTree;
    it.m_travOrd = CBSTree<T>::iterator::EPostOrder;
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::beginInorder()
{
    SBSTreeNode<T>* pNode = m_pBSTree;
    if( pNode != nullptr )
    {
        while( pNode->m_pLeft != nullptr )
        {
            pNode = pNode->m_pLeft;
        }
    }
    CBSTree<T>::iterator it(pNode);
    it.m_pRoot = m_pBSTree;
    it.m_travOrd = CBSTree<T>::iterator::EInOrder;
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::end()
{
    return iterator(nullptr);
}

template <typename T>
void CBSTree<T>::traversePostorder( SBSTreeNode<T>*& i_pNode )
{
    if( i_pNode != nullptr )
    {
        if( i_pNode->m_pLeft != nullptr )
        {
            i_pNode = i_pNode->m_pLeft;
            traversePostorder(i_pNode);
        }
        else if( i_pNode->m_pRight != nullptr )
        {
            i_pNode = i_pNode->m_pRight;
            traversePostorder(i_pNode);
        }
    }
}

template <typename T>
T CBSTree<T>::iterator::operator * () const
{
    return m_pNodeCurr->m_val;
}

template <typename T>
bool CBSTree<T>::iterator::operator == ( iterator& i_other )
{
    bool bEqual = false;
    if( m_pNodeCurr == nullptr && i_other.m_pNodeCurr == nullptr )
    {
        bEqual = true;
    }
    else if( m_pNodeCurr != nullptr && i_other.m_pNodeCurr != nullptr )
    {
        bEqual = (*m_pNodeCurr == *i_other.m_pNodeCurr);
    }
    return bEqual;
}

template <typename T>
bool CBSTree<T>::iterator::operator != ( iterator& i_other )
{
    return !(*this == i_other);
}

template <typename T>
typename CBSTree<T>::iterator& CBSTree<T>::iterator::operator ++ ()
{
    if( m_pNodeCurr == nullptr )
    {
        throw std::exception("iterator out of tree bounds");
    }
    SBSTreeNode<T>* pNodePrev = m_pNodeCurr;
    SBSTreeNode<T>* pNodeNew = nullptr;
    if( m_travOrd == EPreOrder )
    {
        if( m_pNodeCurr->m_pLeft != nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pLeft;
        }
        else if( m_pNodeCurr->m_pRight != nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pRight;
        }
        else if( m_pNodeCurr->m_pUp != nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pUp;
            while( pNodeNew != nullptr )
            {
                if( (pNodeNew->m_pRight != nullptr) && (pNodeNew->m_pRight != pNodePrev) )
                {
                    pNodeNew = pNodeNew->m_pRight;
                    break;
                }
                pNodePrev = pNodeNew;
                pNodeNew = pNodeNew->m_pUp;
            }
        }
        m_pNodeCurr = pNodeNew;
    }
    else if( m_travOrd == EPostOrder )
    {
        if( m_pNodeCurr->m_pLeft == nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pUp;
            if( pNodeNew->m_pRight != nullptr && pNodeNew->m_pRight != pNodePrev )
            {
                pNodeNew = pNodeNew->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
        }
        else
        {
            if( m_pNodeCurr->m_pUp == nullptr )
            {
                if( m_pNodeCurr != m_pRoot )
                {
                    pNodeNew = m_pNodeCurr->m_pRight;
                }
            }
            else if( m_pNodeCurr->m_pUp->m_pRight != nullptr && m_pNodeCurr->m_pUp->m_pRight != pNodePrev )
            {
                pNodeNew = m_pNodeCurr->m_pUp->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
            else
            {
                pNodeNew = m_pNodeCurr->m_pUp;
            }
        }
        m_pNodeCurr = pNodeNew;
    }
    else if( m_travOrd == EInOrder )
    {
        if( m_pNodeCurr->m_pLeft == nullptr )
        {
            if( m_pNodeCurr->m_pRight != nullptr )
            {
                pNodeNew = m_pNodeCurr->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
            else
            {
                if( m_pNodeCurr->m_pUp != nullptr )
                {
                    if( m_pNodeCurr->m_pUp != nullptr && m_pNodeCurr->m_pUp->m_pLeft == m_pNodeCurr )
                    {
                        pNodeNew = m_pNodeCurr->m_pUp;
                    }
                    else
                    {
                        pNodePrev = m_pNodeCurr;
                        pNodeNew = m_pNodeCurr->m_pUp;
                        while( pNodeNew != nullptr )
                        {
                            pNodePrev = pNodeNew;
                            pNodeNew = pNodeNew->m_pUp;
                            if( pNodeNew != nullptr && pNodeNew->m_pLeft == pNodePrev )
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if( m_pNodeCurr->m_pRight != nullptr )
            {
                pNodeNew = m_pNodeCurr->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
            else
            {
                pNodePrev = m_pNodeCurr;
                pNodeNew = m_pNodeCurr->m_pUp;
                while( pNodeNew != nullptr )
                {
                    if( pNodeNew != nullptr && pNodeNew->m_pLeft == pNodePrev )
                    {
                        break;
                    }
                    pNodePrev = pNodeNew;
                    pNodeNew = pNodeNew->m_pUp;
                }
            }
        }
        m_pNodeCurr = pNodeNew;
    }
    return *this;
}

/*******************************************************************************
Entry point for the application.
*******************************************************************************/

#define _ArrLen(_arr) static_cast<int>((sizeof(_arr)/sizeof(_arr[0])))

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    int iResult = 0;

    int iSize;

    std::cout << "\n";
    std::cout << "**************************************************************************\n";
    std::cout << "                  6                                                       \n";
    std::cout << "            4           8                                                 \n";
    std::cout << "         2     5     7     9                                              \n";
    std::cout << "**************************************************************************\n";
    std::cout << "\n";

    CBSTree<int> bt1;

    auto it = bt1.end();

    int ariBT1[] = { 6, 4, 8, 2, 5, 7, 9 };

    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        bt1.add( ariBT1[idxVal] );
    }

    //--------------------------------------------------------------------------
    // Search
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test Search\n";
    std::cout << "-------------\n";

    auto iSearchResult1 = 0;

    //                                                6
    //                                          4           8
    //                                       2     5     7     9
    //int ariBT1[]                      = {  6,  4,  8,  2,  5,  7,  9 };
    int ariSearchValsBT1[]              = {  6,  4,  8,  2,  5,  7,  9 };
    int ariDesiredResultsSearchVals1[]  = {  0,  0,  0,  0,  0,  0,  0 };
    int ariActualResultsSearchVals1[]   = { -1, -1, -1, -1, -1, -1, -1 };
    #ifdef _DEBUG
    int ariDesiredResultsSearchSteps1[] = {  0,  1,  1,  2,  2,  2,  2 };
    int ariActualResultsSearchSteps1[]  = { -1, -1, -1, -1, -1, -1, -1 };
    #endif

    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT1); ++idxVal )
    {
        it = bt1.search(ariSearchValsBT1[idxVal]);

        if( it == bt1.end() )
        {
            ariActualResultsSearchVals1[idxVal] = -1;
        }
        else
        {
            ariActualResultsSearchVals1[idxVal] = 0;

            #ifdef _DEBUG
            ariActualResultsSearchSteps1[idxVal] = it.m_iSearchSteps;
            #endif
        }
        if( ariActualResultsSearchVals1[idxVal] != ariDesiredResultsSearchVals1[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #ifdef _DEBUG
        if( ariActualResultsSearchSteps1[idxVal] != ariDesiredResultsSearchSteps1[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #endif
    }
    if( iResult == 0 )
    {
        iResult = iSearchResult1;
    }

    std::cout << "Search Values:          ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariSearchValsBT1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Results: ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchVals1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Results:  ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchVals1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Steps:   ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchSteps1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Steps:    ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchSteps1[idxVal];
    }
    std::cout << "\n";

    if( iSearchResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // Preorder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PreOrder\n";
    std::cout << "-------------\n";

    auto iPreorderResult1 = 0;

    //                                             6
    //                                       4           8
    //                                    2     5     7     9
    int ariDesiredResultsPreorder1[] = {  6,  4,  2,  5,  8,  7,  9 };
    int ariActualResultsPreorder1[]  = { -1, -1, -1, -1, -1, -1, -1 };

    it = bt1.beginPreorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt1.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPreorder1[idxVal] = *it;

        if( ariActualResultsPreorder1[idxVal] != ariDesiredResultsPreorder1[idxVal] )
        {
            iPreorderResult1 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT1) )
    {
        iPreorderResult1 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPreorderResult1;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT1) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPreorder1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPreorder1[idxVal];
    }
    std::cout << "\n";

    if( iPreorderResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // PostOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PostOrder\n";
    std::cout << "--------------\n";

    auto iPostorderResult1 = 0;

    //                                              6
    //                                        4           8
    //                                     2     5     7     9
    //int ariBT1[]                    = {  6,  4,  8,  2,  5,  7,  9 };
    int ariDesiredResultsPostorder1[] = {  2,  5,  4,  7,  9,  8,  6 };
    int ariActualResultsPostorder1[]  = { -1, -1, -1, -1, -1, -1, -1 };

    it = bt1.beginPostorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt1.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPostorder1[idxVal] = *it;

        if( ariActualResultsPostorder1[idxVal] != ariDesiredResultsPostorder1[idxVal] )
        {
            iPostorderResult1 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT1) )
    {
        iPostorderResult1 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPostorderResult1;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT1) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPostorder1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPostorder1[idxVal];
    }
    std::cout << "\n";

    if( iPostorderResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // InOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test InOrder\n";
    std::cout << "------------\n";

    auto iInorderResult1 = 0;

    //                                                    6
    //                                              4           8
    //                                           2     5     7     9
    //int ariBT1[]                  = {  6,  4,  8,  2,  5,  7,  9 };
    int ariDesiredResultsInorder1[] = {  2,  4,  5,  6,  7,  8,  9 };
    int ariActualResultsInorder1[]  = { -1, -1, -1, -1, -1, -1, -1 };

    it = bt1.beginInorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt1.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsInorder1[idxVal] = *it;

        if( ariActualResultsInorder1[idxVal] != ariDesiredResultsInorder1[idxVal] )
        {
            iInorderResult1 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT1) )
    {
        iInorderResult1 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iInorderResult1;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT1) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsInorder1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsInorder1[idxVal];
    }
    std::cout << "\n";

    if( iInorderResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    std::cout << "\n";
    std::cout << "**************************************************************************\n";
    std::cout << "                 59                                                       \n";
    std::cout << "         49                  81                                           \n";
    std::cout << "     45                  79                                               \n";
    std::cout << "         46          77                                                   \n";
    std::cout << "             47                                                           \n";
    std::cout << "**************************************************************************\n";
    std::cout << "\n";

    CBSTree<int> bt2;

    int ariBT2[] = { 59, 81, 79, 77, 49, 45, 46, 47 };

    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        bt2.add( ariBT2[idxVal] );
    }

    //--------------------------------------------------------------------------
    // Search
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test Search\n";
    std::cout << "-------------\n";

    auto iSearchResult2 = 0;

    //                                                  59            
    //                                          49                  81
    //                                      45                  79    
    //                                          46          77        
    //                                              47                
    //int ariBT2[]                      = { 59, 81, 79, 77, 49, 45, 46, 47 };
    int ariSearchValsBT2[]              = { 59, 81, 79, 77, 49, 45, 46, 47 };
    int ariDesiredResultsSearchVals2[]  = {  0,  0,  0,  0,  0,  0,  0,  0 };
    int ariActualResultsSearchVals2[]   = { -1, -1, -1, -1, -1, -1, -1, -1 };
    #ifdef _DEBUG
    int ariDesiredResultsSearchSteps2[] = {  0,  1,  2,  3,  1,  2,  3,  4 };
    int ariActualResultsSearchSteps2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };
    #endif

    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT2); ++idxVal )
    {
        it = bt2.search(ariSearchValsBT2[idxVal]);

        if( it == bt2.end() )
        {
            ariActualResultsSearchVals2[idxVal] = -1;
        }
        else
        {
            ariActualResultsSearchVals2[idxVal] = 0;

            #ifdef _DEBUG
            ariActualResultsSearchSteps2[idxVal] = it.m_iSearchSteps;
            #endif
        }
        if( ariActualResultsSearchVals2[idxVal] != ariDesiredResultsSearchVals2[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #ifdef _DEBUG
        if( ariActualResultsSearchSteps2[idxVal] != ariDesiredResultsSearchSteps2[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #endif
    }
    if( iResult == 0 )
    {
        iResult = iSearchResult1;
    }

    std::cout << "Search Values:          ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariSearchValsBT2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Results: ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchVals2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Results:  ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchVals2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Steps:   ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchSteps2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Steps:    ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchSteps2[idxVal];
    }
    std::cout << "\n";

    if( iSearchResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // Preorder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PreOrder\n";
    std::cout << "-------------\n";

    auto iPreorderResult2 = 0;

    //                                                  59            
    //                                          49                  81
    //                                      45                  79    
    //                                          46          77        
    //                                              47                
    int ariDesiredResultsPreorder2[] = { 59, 49, 45, 46, 47, 81, 79, 77 };
    int ariActualResultsPreorder2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };

    it = bt2.beginPreorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt2.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPreorder2[idxVal] = *it;

        if( ariActualResultsPreorder2[idxVal] != ariDesiredResultsPreorder2[idxVal] )
        {
            iPreorderResult2 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT2) )
    {
        iPreorderResult2 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPreorderResult2;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT2) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPreorder2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPreorder2[idxVal];
    }
    std::cout << "\n";

    if( iPreorderResult2 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // PostOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PostOrder\n";
    std::cout << "--------------\n";

    auto iPostorderResult2 = 0;

    //                                                  59            
    //                                          49                  81
    //                                      45                  79    
    //                                          46          77        
    //                                              47                
    int ariDesiredResultsPostorder2[] = { 47, 46, 45, 49, 77, 79, 81, 59 };
    int ariActualResultsPostorder2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };

    it = bt2.beginPostorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt2.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPostorder2[idxVal] = *it;

        if( ariActualResultsPostorder2[idxVal] != ariDesiredResultsPostorder2[idxVal] )
        {
            iPostorderResult2 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT2) )
    {
        iPostorderResult2 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPostorderResult2;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT2) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPostorder2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPostorder2[idxVal];
    }
    std::cout << "\n";

    if( iPostorderResult2 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // InOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test InOrder\n";
    std::cout << "------------\n";

    auto iInorderResult2 = 0;

    //                                                  59            
    //                                          49                  81
    //                                      45                  79    
    //                                          46          77        
    //                                              47                
    int ariDesiredResultsInorder2[] = { 45, 46, 47, 49, 59, 77, 79, 81 };
    int ariActualResultsInorder2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };

    it = bt2.beginInorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt2.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsInorder2[idxVal] = *it;

        if( ariActualResultsInorder2[idxVal] != ariDesiredResultsInorder2[idxVal] )
        {
            iInorderResult2 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT2) )
    {
        iInorderResult2 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iInorderResult2;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT2) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsInorder2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsInorder2[idxVal];
    }
    std::cout << "\n";

    if( iInorderResult2 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    return iResult;

} // main
