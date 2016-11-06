#ifndef CandidateNtuplizer_H
#define CandidateNtuplizer_H

#include "../interface/NtupleBranches.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

using namespace std;

class CandidateNtuplizer{

public:
   CandidateNtuplizer( NtupleBranches* nBranches );
   virtual ~CandidateNtuplizer( void );

   virtual void fillBranches( edm::Event const & event, const edm::EventSetup& iSetup ){};
   
protected:
   NtupleBranches* nBranches_;
      
};

#endif // CandidateNtuplizer_H
