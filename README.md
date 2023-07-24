# ADTS
When storing real-world data, lists containing other lists are ubiquitous e.g. : (0 (1 2) 3 4 5). In fact many functional (or at least partly functional) languages are based around the idea that all data are, conceptually, nested lists. Examples of such languages include Haskell and, of interest here, Lisp :

One of the main operations of these languages is the ability to extract the front (head) or remainder (tail) of the list very efficiently. A traditional linked list is not good for this, since it would require work to untangle the head (it has a pointer to the rest of the list) and it’s unclear how a list-within-a-list would be stored.
For Lisp, a better structure was developed, which is slightly more complex than a traditional linked list, but which makes the extraction of the head (and remainder) simple and fast. To encode the list (1 2), which is a list containing two atoms, we use :
<img width="197" alt="image" src="https://github.com/Leon-Chen1999/ADTS/assets/122807406/d529387c-0243-4614-a596-8784b9b6a2ef">  
Here the main struct (known as the cons) has two pointers. The ones going downwards known as the car pointers and the ones going horizontally the cdr1 pointers. If we were to have a pointer p to the first cons of this list, the head of the list is pointed to by p->car, and the remainder of the list simply by p->cdr.
A more complex list, (0 (1 2) 3 4 5) would be stored as :  
<img width="391" alt="image" src="https://github.com/Leon-Chen1999/ADTS/assets/122807406/071120b9-2713-41ee-8c80-ec64894fe347">  
The atomic elements (the integers) are stored as ‘leaf’ nodes with both car and cdr pointers set to NULL. The data structure used to store the atom and also car and cdr pointers is known as a cons (short for constructor), due the to process by which we build lists.
So, here, we’re interested in recreating this data structure, allowing the user to build lists (the cons operation), extract the head and remainder of lists (the car and cdr operations) and other associated functions such as copying a list or counting the number of elements in it.  
<img width="423" alt="image" src="https://github.com/Leon-Chen1999/ADTS/assets/122807406/e16fad15-4d50-4b3b-9b35-3920921b2fb9">  
