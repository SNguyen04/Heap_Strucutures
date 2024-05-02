/ UMBC - CMSC 341 - Spring 2024 - Proj3                                                                                                                                     
#include "rqueue.h"

//Name: RQueue (Constructor)                                                                                                                                                 
//Desc: Creates empty queue and initializes values                                                                                                                           
//Preconditions: required variables provided                                                                                                                                 
//Postconditions: Empty queue is created with intialized values                                                                                                              
RQueue::RQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) {
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
}



//Name: Destructor                                                                                                                                                           
//Desc: Deallocates all memory and re-initializaed the member variables                                                                                                      
//Preconditions: Heap exists                                                                                                                                                 
//Postconditions: All memory is deallocated and member variables reinitialized                                                                                               
RQueue::~RQueue() {
  clear();
}



//Name: Destructor                                                                                                                                                           
//Desc: Deallocates all nodes and re-initializaed the member variables                                                                                                       
//Preconditions: Heap exists                                                                                                                                                 
//Postconditions: All nodes are deallocated and member variables reinitialized                                                                                               
//leaving an empty heap                                                                                                                                                      
void RQueue::clear() {
  clear(m_heap);
  m_size = 0;
}



//Name: RQueue (Copy Constructor)                                                                                                                                            
//Desc: creates a deep copy of rhs object                                                                                                                                    
//Preconditions: Heap exist                                                                                                                                                  
//Postconditions: a deep copy of heap is created                                                                                                                             
RQueue::RQueue(const RQueue& rhs) {
  //copies mem variables                                                                                                                                                     
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;

  //if root exists                                                                                                                                                           
  if(rhs.m_heap != nullptr){
    //recursive copy                                                                                                                                                         
    copy(m_heap, rhs.m_heap);

    //in case empty heap                                                                                                                                                     
  }else{
    m_heap = nullptr;
  }
}

//Name: getHeapType                                                                                                                                                          
//Desc: returns heap type                                                                                                                                                    
//Preconditions: tree exists with defined heap type                                                                                                                          
//Postconditions: heap type is returned                                                                                                                                      
HEAPTYPE RQueue::getHeapType() const {
  return m_heapType;
}



//Name: operator= (Assignment Operator)                                                                                                                                      
//Desc: creates a deep copy of rhs object                                                                                                                                    
//Preconditions: Heap exist                                                                                                                                                  
//Postconditions: a deep copy of heap is created                                                                                                                             
RQueue& RQueue::operator=(const RQueue& rhs) {
  //protection against self-assignment                                                                                                                                       
  if(this == &rhs){
    return *this;
  }

  //clears lhs                                                                                                                                                               
  clear();

  //copies mem variables                                                                                                                                                     
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;

  //if root exist                                                                                                                                                            
  if(rhs.m_heap != nullptr){
    //recursive copy                                                                                                                                                         
    copy(m_heap, rhs.m_heap);

    //in case empty heap                                                                                                                                                     
  }else{
    m_heap = nullptr;
  }

  return *this;

}



//Name: mergeWithQueue                                                                                                                                                       
//Desc: merges host queu with rhs, leaving the rhs queue empty                                                                                                               
//Preconditions: two heaps exist with same priority/structure                                                                                                                
//Postconditions: the two queues are merged while maintained heap/struc                                                                                                      
//property, rhs will become empty                                                                                                                                            
void RQueue::mergeWithQueue(RQueue& rhs) {
  //protects from merging heap with itself                                                                                                                                   
  //checks if rhs is empty                                                                                                                                                   
  if(this != &rhs || rhs.m_heap == nullptr){

    //makes sure same priority and structure                                                                                                                                 
    if(m_priorFunc == rhs.m_priorFunc
       && m_heapType == rhs.m_heapType
       && m_structure == rhs.m_structure){
     //if host is empty but rhs is not                                                                                                                                      
      if(m_heap == nullptr){
        //points m_heap to rhs heap and update mem vars                                                                                                                      
        m_heap = rhs.m_heap;
        m_size = rhs.m_size;


        //both heaps exists and are non-empty                                                                                                                                
      }else{

        //Min type                                                                                                                                                           
        if(m_heapType == 0){
          m_heap = mergeMin(this->m_heap, rhs.m_heap);

          //Max type                                                                                                                                                         
        }else{
          m_heap = mergeMax(this->m_heap, rhs.m_heap);
        }

        //update mem variables                                                                                                                                               
        m_size = m_size + rhs.m_size;
      }

      //clears rhs to empty object                                                                                                                                           
      rhs.m_heap = nullptr;
      rhs.m_size = 0;

      //throws exception error                                                                                                                                               
    }else{
      throw domain_error("Heaps do not share the same priority or structure.");
    }
  }
}



//Name: insertStudent                                                                                                                                                        
//Desc: Insert Student into queue                                                                                                                                            
//Preconditions: Heap and student exist                                                                                                                                      
//Postconditions: Inserts Student into heap while keeping heap type/property                                                                                                 
void RQueue::insertStudent(const Student& student) {
  //if queue/root is empty                                                                                                                                                   
  if(m_heap == nullptr){
    //creates first root node                                                                                                                                                
    m_heap = new Node(student);

    //root exists, must insert into queue through merge                                                                                                                      
  }else{
    Node* temp = new Node(student);

    //Min type                                                                                                                                                               
    if(m_heapType == 0){
      m_heap = mergeMin(m_heap, temp);
      //Max type                                                                                                                                                             
    }else{
      m_heap = mergeMax(m_heap, temp);
    }
  }

  m_size++;
}

//Name: numStudents                                                                                                                                                          
//Desc: returns current num of students in queue                                                                                                                             
//Preconditions: heap exists                                                                                                                                                 
//Postconditions: returns int num of students/nodes in heap                                                                                                                  
int RQueue::numStudents() const {
  return m_size;
}



//Name: getPriorityFn                                                                                                                                                        
//Desc: returns current priority function                                                                                                                                    
//Preconditions: heap exists with initializaed priority function                                                                                                             
//Postconditions: returns current priority function                                                                                                                          
prifn_t RQueue::getPriorityFn() const {
  return m_priorFunc;
}



//Name: getNextStudent                                                                                                                                                       
//Desc: Removes and return highest priority student from heap                                                                                                                
//Preconditions: Heap exist with nodes                                                                                                                                       
//Postconditions: Highest priority Student is returned while maintaining                                                                                                     
//heap type/property                                                                                                                                                         
Student RQueue::getNextStudent() {
  //checks if heap is empty                                                                                                                                                  
  if(m_size != 0){

    Node* temp = m_heap; //Node ptr holding onto root data                                                                                                                   

    //Min type                                                                                                                                                               
    if(m_heapType == 0){
      m_heap = mergeMin(temp->m_left, temp->m_right);

      //Max type                                                                                                                                                             
    }else{
      m_heap = mergeMax(temp->m_left, temp->m_right);
    }

    Student studentData = temp->getStudent(); //Student data to be returned                                                                                                  

    //deletes root                                                                                                                                                           
    delete temp;
    temp = nullptr;

    //update mem variable                                                                                                                                                    
    m_size--;

    return studentData;

    //throws exception if heap is empty                                                                                                                                      
  }else{
    throw out_of_range("Heap is empty");
  }
}

//Name: setPriorityFn                                                                                                                                                        
//Desc: sets a new priority function and corresponding heap type                                                                                                             
//Preconditions: Heap exists and given compatible arguments                                                                                                                  
//Postconditions: Heap is rebuild to follow new priority and heaptype                                                                                                        
void RQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  bool propertyCheck; //bool to check if heaptype property is correct                                                                                                        

  //sets priority function and heap type                                                                                                                                     
  m_priorFunc = priFn;
  m_heapType = heapType;

  //runs until priority/heaptype property is fulfilled                                                                                                                       
  do{
    //resets bool to true                                                                                                                                                    
    propertyCheck = true;
    //traverses through heap and check each node to ensure property is upheld                                                                                                
    reHeapify(m_heap, propertyCheck);
  }while(!propertyCheck);
}



//Name:setStructure                                                                                                                                                          
//Desc: Changes structure of heap and rebuilds if necessary                                                                                                                  
//Preconditions: heap exist and structure given as argument                                                                                                                  
//Postconditions: structure is changed and heap is rebuilt accordingly                                                                                                       
void RQueue::setStructure(STRUCTURE structure){

  //sets structure type                                                                                                                                                      
  m_structure = structure;

  Node* temp = m_heap; //Node ptr temporilary pointing to m_heap                                                                                                             
  m_heap = nullptr;
  //rebuilds heap according to structure                                                                                                                                     
  reStructure(m_heap, temp);
}



//Name: getStructure                                                                                                                                                         
//Desc: returns structure type                                                                                                                                               
//Preconditions: heap exists with defined structure type                                                                                                                     
//Postconditions: structure type is returned                                                                                                                                 
STRUCTURE RQueue::getStructure() const {
  return m_structure;
}



//Name: printStudentsQueue                                                                                                                                                   
//Decs: prints students in heap                                                                                                                                              
//Preconditions: heap exists with nodes/students                                                                                                                             
//Postconditions: Students in heap are printed with necessary data,                                                                                                          
//starting with student with most priority than follows preorder traversal                                                                                                   
void RQueue::printStudentsQueue() const {
  if(m_size == 0){
    cout << "Empty heap.\n";

  }else{
    cout << "Contents of the queue:\n";
    print(m_heap);
  }

}



void RQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}



void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name;
    else
        cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student name: " << student.m_name
        << ", Major: " << student.getMajorStr()
        << ", Gender: " << student.getGenderStr()
        << ", Level: " << student.getLevelStr();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.m_student;
  return sout;
}


//***********Helper Functions**********                                                                                                                                      



//Name: clear (recursive)                                                                                                                                                    
//Desc: deletes all nodes in the heap and reinitializes the mem variables                                                                                                    
//Preconditions: heap exists with nodes                                                                                                                                      
//Postconditions: all nodes are deallocated, leaving an empty heap                                                                                                           
void RQueue::clear(Node* &aNode){
  if(aNode != nullptr){
    //checks left nodes                                                                                                                                                      
    clear(aNode->m_left);

    //checks right nodes                                                                                                                                                     
    clear(aNode->m_right);

    delete aNode;
    aNode = nullptr;
  }
}
//Name: mergeMax                                                                                                                                                             
//Desc: merges two heaps together                                                                                                                                            
//Preconditions: two heaps exists to be merged                                                                                                                               
//Postconditions: heaps are merged while maintaining heap property and                                                                                                       
//following a max heap structure                                                                                                                                             
Node* RQueue::mergeMax(Node* &heap1, Node* &heap2){

  //heap1 is empty                                                                                                                                                           
  if(heap1 == nullptr){
    return heap2;

    //heap2 is empty                                                                                                                                                         
  }else if(heap2 == nullptr){
    return heap1;

    //if heap1 priority is greater than heap2                                                                                                                                
  }else if(m_priorFunc(heap1->getStudent()) >= m_priorFunc(heap2->getStudent())){

    //merges heap2 into heap1                                                                                                                                                
    heap1->m_right = mergeMax(heap1->m_right, heap2);

    //update npl                                                                                                                                                             
    updateNPL(heap2);
    updateNPL(heap1);

    //checks for structure type                                                                                                                                              
    if(m_structure == LEFTIST){

      //checks if property is managed                                                                                                                                        
      if(!leftistPropertyCheckNode(heap1)){
        //swaps nodes                                                                                                                                                        
        swap(heap1);
      }

      //swaps regardless -> skewed heap                                                                                                                                      
    }else{
      swap(heap1);
    }

    return heap1;


  }else{
    //merges heap1 into heap2                                                                                                                                                
    heap2->m_right = mergeMax(heap2->m_right, heap1);

    //update npl                                                                                                                                                             
    updateNPL(heap1);
    updateNPL(heap2);

    //checks for structure type                                                                                                                                              
    if(m_structure == LEFTIST){

      //checks if property is managed                                                                                                                                        
      if(!leftistPropertyCheckNode(heap2)){
        //swaps nodes                                                                                                                                                        
        swap(heap2);
      }

      //swaps regardless -> skewed heap                                                                                                                                      
    }else{
      swap(heap2);
      }

      //swaps regardless -> skewed heap                                                                                                                                      
    }else{
      swap(heap2);
    }

    return heap2;
  }
}



//Name: mergeMin                                                                                                                                                             
//Desc: merges two heaps together                                                                                                                                            
//Preconditions: two heaps exists to be merged                                                                                                                               
//Postconditions: heaps are merged while maintaining heap property and                                                                                                       
//following a min heap structure                                                                                                                                             
Node* RQueue::mergeMin(Node* &heap1, Node* &heap2){

  //heap1 is empty                                                                                                                                                           
  if(heap1 == nullptr){
    return heap2;

    //heap2 is empty                                                                                                                                                         
  }else if(heap2 == nullptr){
    return heap1;

    //if heap1 priority is less than heap2 (min heap)                                                                                                                        
  }else if(m_priorFunc(heap1->getStudent()) <= m_priorFunc(heap2->getStudent())){

    //merges heap2 into heap1                                                                                                                                                
    heap1->m_right = mergeMin(heap1->m_right, heap2);

    //update npl                                                                                                                                                             
    updateNPL(heap2);
    updateNPL(heap1);

    //checks for structure type                                                                                                                                              
    if(m_structure == LEFTIST){

      //checks if property is managed                                                                                                                                        
      if(!leftistPropertyCheckNode(heap1)){
        //swaps nodes                                                                                                                                                        
        swap(heap1);
      }

      //swaps regardless -> skewed heap                                                                                                                                      
    }else{
      swap(heap1);
    }

    return heap1;


  }else{
    //merges heap1 into heap2                                                                                                                                                
    heap2->m_right = mergeMin(heap2->m_right, heap1);

    //update npl                                                                                                                                                             
    updateNPL(heap1);
    updateNPL(heap2);

    //checks for structure type                                                                                                                                              
    if(m_structure == LEFTIST){

      //checks if property is managed                                                                                                                                        
      if(!leftistPropertyCheckNode(heap2)){
        //swaps nodes                                                                                                                                                        
        swap(heap2);
      }

      //swaps regardless -> skewed heap                                                                                                                                      
    }else{
      swap(heap2);
    }

    return heap2;
  }
}



//Name: swap                                                                                                                                                                 
//Desc: swaps the children of the given node                                                                                                                                 
//Preconditions: node exists with at least one child                                                                                                                         
//Postconditions: swaps the node's children                                                                                                                                  
void RQueue::swap(Node* &heap){
  Node* temp = heap->m_left;
  heap->m_left = heap->m_right;
  heap->m_right = temp;
}



//Name: swap                                                                                                                                                                 
//Desc: swaps a child student with its parent node                                                                                                                           
//Preconditions: node exists with at least one child                                                                                                                         
//Postconditions: swaps the node's student with its child                                                                                                                    
//nodes stay in place, data swaps                                                                                                                                            
void RQueue::swap(Node* &parent, Node* &child){
  Student temp = child->getStudent();
  child->m_student = parent->getStudent();
  parent->m_student = temp;
}



//Name: updateNPL                                                                                                                                                            
//Desc: updates npl of node                                                                                                                                                  
//Preconditions: Node exist                                                                                                                                                  
//Postcondition: npl is updated                                                                                                                                              
void RQueue::updateNPL(Node* &aNode){
  aNode->setNPL(findMin(aNode) + 1);
}
//Name: findMin                                                                                                                                                              
//Desc: finds shortest between children                                                                                                                                      
//Preconditions: tree exists                                                                                                                                                 
//Postconditions: returns int shortest npl                                                                                                                                   
int RQueue::findMin(Node* &aNode){
  int leftChild = -1; //int represent left child NPL                                                                                                                         
  int rightChild = -1; //int represent right child NPL                                                                                                                       

  //checks if left child exists                                                                                                                                              
  if(aNode->m_left != nullptr){
    leftChild = aNode->m_left->getNPL();
  }

  //checks if right child exists                                                                                                                                             
  if(aNode->m_right != nullptr){
    rightChild = aNode->m_right->getNPL();
  }

  //finds min between two npl                                                                                                                                                
  if(leftChild < rightChild){
    return leftChild;
  }else{
    return rightChild;
  }
}



//Name: leftistPropertyCheck                                                                                                                                                 
//Desc: checks whether the leftist property at ONE node is maintained                                                                                                        
//Preconditions: heap is leftist structure and exists                                                                                                                        
//Postconditions: returns true if property is maintained, false if not                                                                                                       
bool RQueue::leftistPropertyCheckNode(Node* &aNode){
  int leftChild = -1; //int of left child NPL                                                                                                                                
  int rightChild = -1; //int of right child NPL                                                                                                                              

  //checks if left child exists                                                                                                                                              
  if(aNode->m_left != nullptr){
    leftChild = aNode->m_left->getNPL();
  }

  //checks if right child exists                                                                                                                                             
  if(aNode->m_right != nullptr){
    rightChild = aNode->m_right->getNPL();
  }

  //checks whether left NPL is greater than right NPL                                                                                                                        
  if(leftChild >= rightChild){
    return true;
  }else{
    return false;
  }
}

//Name: copy (recursive)                                                                                                                                                     
//Desc: Traverses through both heaps and creates a deep copy (pre-order)                                                                                                     
//Preconditions: Two heaps exists                                                                                                                                            
//Postconditions: Deep copy of heap is made                                                                                                                                  
void RQueue::copy(Node* &aNode, Node* rhs){
  //if rhs != null, creates node                                                                                                                                             
  if(rhs != nullptr){
    //copies student data                                                                                                                                                    
    aNode = new Node(rhs->getStudent());

    aNode->setNPL(rhs->getNPL());

    //move left                                                                                                                                                              
    copy(aNode->m_left, rhs->m_left);

    //move right                                                                                                                                                             
    copy(aNode->m_right, rhs->m_right);
  }
}



//Name: print (recursive)                                                                                                                                                    
//Desc: traverses through heap and prints students in queue                                                                                                                  
//Preconditions: Heap exist with students                                                                                                                                    
//Postconditions: Students from queue are printed starting with highest                                                                                                      
//priority student                                                                                                                                                           
void RQueue::print(Node* aNode) const {
  //checks if node is empty                                                                                                                                                  
  if(aNode != nullptr){

    cout << "[" << m_priorFunc(aNode->getStudent()) << "] "
         << aNode->getStudent() << endl;

    //move left                                                                                                                                                              
    print(aNode->m_left);

    //move right                                                                                                                                                             
    print(aNode->m_right);

  }
}



//Name: reHeapify                                                                                                                                                            
//Desc: traverses through heap and rebuilds the heap                                                                                                                         
//Preconditions: Heap exist and given a bool passed by ref                                                                                                                   
//Postconditions: Checks if heap needs to be rebuilt or not; if it needs to                                                                                                  
//be rebuilt, swaps are made whlie changing bool to false                                                                                                                    
//runs until no swaps are done                                                                                                                                               
void RQueue::reHeapify(Node* aNode, bool &check){

  //if node is not empty                                                                                                                                                     
  if(aNode != nullptr){
    //move left                                                                                                                                                              
    reHeapify(aNode->m_left, check);

    //move right                                                                                                                                                             
    reHeapify(aNode->m_right, check);
    int leftChild = -1; //int holding left child priority                                                                                                                    
    int rightChild = -1; //int holding right child priority                                                                                                                  

    //checks if left child exist                                                                                                                                             
    if(aNode->m_left != nullptr){
      leftChild = m_priorFunc(aNode->m_left->getStudent());
    }

    //checks if right child exist                                                                                                                                            
    if(aNode->m_right != nullptr){
      rightChild = m_priorFunc(aNode->m_right->getStudent());
    }


    //Min heap                                                                                                                                                               
    if(m_heapType == 0){

      //checks if left child exist and if child has higher priority than parent                                                                                              
      //(child priority is smaller than parent priority -> swap)                                                                                                             
      if(leftChild != -1 && leftChild < m_priorFunc(aNode->getStudent())){
        swap(aNode, aNode->m_left);
        check = false;
      }

      //checks if right child exist and if child has higher priority than parent                                                                                             
      //(child priority is smaller than parent priority -> swap)                                                                                                             
      if(rightChild != -1 && rightChild < m_priorFunc(aNode->getStudent())){
        swap(aNode, aNode->m_right);
        check = false;
      }

      //Max heap                                                                                                                                                             
    }else{

      //checks if left child exist and if child has higher priority than parent                                                                                              
      //(child priority is bigger than parent priority -> swap)                                                                                                              
      if(leftChild != -1 && leftChild > m_priorFunc(aNode->getStudent())){
        swap(aNode, aNode->m_left);
        check = false;
      }

      //checks if right child exist and if child has higher priority than parent                                                                                             
      //(child priority is bigger than parent priority -> swap)                                                                                                              
      if(rightChild != -1 && rightChild > m_priorFunc(aNode->getStudent())){
        swap(aNode, aNode->m_right);
        check = false;
      }
    }
  }
}

//Name: reStructure                                                                                                                                                          
//Desc: restructures heap according to new structure type                                                                                                                    
//Preconditions: Two Node ptrs, one to the host heap and a ptr to the                                                                                                        
//original heap                                                                                                                                                              
//Postconditions: Nodes are reorganized according to structure type                                                                                                          
void RQueue::reStructure(Node* &host, Node* &temp){

  //checks if node is empty                                                                                                                                                  
  if(temp != nullptr){

    //move left                                                                                                                                                              
    reStructure(host, temp -> m_left);

    //move right                                                                                                                                                             
    reStructure(host, temp -> m_right);

    //inserts first node into heap if empty                                                                                                                                  

    //Min heap                                                                                                                                                               
    if(m_heapType == 0){
      host = mergeMin(host, temp);

      //Max type                                                                                                                                                             
    }else{
      host = mergeMax(host, temp);
    }

    temp = nullptr;
  }
}

