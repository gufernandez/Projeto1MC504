/*The roller coaster problem

This problem is from Andrews’s Concurrent Programming [1], but he attributes it
to J. S. Herman’s Master’s thesis. Suppose there are n passenger threads and a
car thread. The passengers repeatedly wait to take rides in the car, which can
hold C passengers, where C < n.

The car can go around the tracks only when it is full. Here are some additional
details:

• Passengers should invoke board and unboard.
• The car should invoke load, run and unload.
• Passengers cannot board until the car has invoked load
• The car cannot depart until C passengers have boarded.
• Passengers cannot unboard until the car has invoked unload.

Puzzle:
Write code for the passengers and car that enforces these constraints.*/
/*============================================================================*/

/*5.8.1 Roller Coaster hint*/

mutex = Semaphore (1);
mutex2 = Semaphore (1);
boarders = 0;
unboarders = 0;
boardQueue = Semaphore (0);
unboardQueue = Semaphore (0);
allAboard = Semaphore (0);
allAshore = Semaphore (0);

/*mutex protects passengers, which counts the number of passengers that have
invoked boardCar. Passengers wait on boardQueue before boarding and unboardQueue
before unboarding. allAboard indicates that the car is full.*/
/*============================================================================*/

/*5.8.2 Roller Coaster solution
Here is my code for the car thread:

Roller Coaster solution (car) */

load ();
boardQueue.signal ( C );
allAboard.wait ();

run ();

unload ();
unboardQueue.signal ( C );
allAshore.wait ();

/*When the car arrives, it signals C passengers, then waits for the last one to
signal allAboard. After it departs, it allows C passengers to disembark,
then waits for allAshore.

Roller Coaster solution (passenger) */

boardQueue.wait ();
board ();

mutex.wait ();
boarders += 1;
if (boarders == C){
  allAboard . signal ();
  boarders = 0;
}
mutex.signal ();

unboardQueue.wait ();
unboard ();

mutex2.wait ();
unboarders += 1;
if (unboarders == C){
  allAshore.signal ();
  unboarders = 0;
}
mutex2.signal ();

/*Passengers wait for the car before boarding, naturally, and wait for the car to
stop before leaving. The last passenger to board signals the car and resets the
passenger counter.*/

/*============================================================================*/

/*5.8.3 Multi-car Roller Coaster problem
This solution does not generalize to the case where there is more than one car.
In order to do that, we have to satisfy some additional constraints:
• Only one car can be boarding at a time.
• Multiple cars can be on the track concurrently.
• Since cars can’t pass each other, they have to unload in the same order they
boarded.
• All the threads from one carload must disembark before any of the threads from
 subsequent carloads.

 Puzzle:
 modify the previous solution to handle the  additional constraints. You can
 assume that there are m cars, and that each car has a local variable named i
 that contains an identifier between 0 and m − 1.*/

/*============================================================================*/

/* 5.8.4 Multi-car Roller Coaster hint I used two lists of semaphores to keep
the cars in order. One represents the loading area and one represents the
unloading area. Each list contains one semaphore for each car. At any time, only
one semaphore in each list is unlocked, so that enforces the order threads can
load and unload. Initially, only the semaphores for Car 0 are unlocked. As each
car enters the loading (or unloading) it waits on its own semaphore; as it
leaves it signals the next car in line.

Multi-car Roller Coaster hint */
loadingArea = [ Semaphore (0) for i in range ( m )];
loadingArea[1].signal ();
unloadingArea = [ Semaphore (0) for i in range ( m )];
unloadingArea[1].signal ();

/*The function next computes the identifier of the next car in the sequence
(wrapping around from m − 1 to 0):

Implementation of next*/

def next ( i ):
  return ( i + 1) % m

/*============================================================================*/

/*5.8.5 Multi-car Roller Coaster solution Here is the modified code for the
cars:
Multi-car Roller Coaster solution (car)*/
loadingArea[i].wait ();
load ();
boardQueue.signal ( C );
allAboard.wait ();
loadingArea[next ( i )].signal ();

run ();

unloadingArea [ i ].wait();
unload ();
unboardQueue.signal ( C );
allAshore.wait ();
unloadingArea[ next ( i )].signal ();

/*The code for the passengers is unchanged.*/
