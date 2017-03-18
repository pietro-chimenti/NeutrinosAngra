//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation implementation pf physics list file
//
//  Authors: P.Chimenti, R.Lima,  K. L. Ribeiro, G. Valdiviesso
//
//  30-4-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//   In this file are set the main classes which create the interacting
//  particles in the detector and the physical processes they participate 
//==============================================================


#include "AngraPhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include "iomanip"

#include "G4IonConstructor.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"
#include "G4RadioactiveDecay.hh"
#include "G4NeutronInelasticProcess.hh"


AngraPhysicsList::AngraPhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 0.010*mm;
  SetVerboseLevel(1);
}

AngraPhysicsList::~AngraPhysicsList()
{
}

void AngraPhysicsList::ConstructParticle()
{
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructIons();

}

void AngraPhysicsList::ConstructBosons()
{
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  G4Gamma::GammaDefinition();
  G4OpticalPhoton::OpticalPhotonDefinition();
}

#include "G4LeptonConstructor.hh"
void AngraPhysicsList::ConstructLeptons()
{
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

#include "G4MesonConstructor.hh"
void AngraPhysicsList::ConstructMesons()
{
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

#include "G4BaryonConstructor.hh"
void AngraPhysicsList::ConstructBaryons()
{
  G4BaryonConstructor  pConstructor;
  pConstructor.ConstructParticle();  
}

#include "G4IonConstructor.hh"
void AngraPhysicsList::ConstructIons()
{
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();  
}


void AngraPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructHadronic();
  ConstructGeneral();
  ConstructOp();
  ConstructNuclear();
  SetCuts();
  AddLimits();
}


#include "G4Decay.hh"

void AngraPhysicsList::ConstructGeneral()
{
  //
  // declare particle decay process for each applicable particle
  //
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager->AddDiscreteProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
  
  //
  //
  // Declare radioactive decay to the GenericIon in the IonTable.
  //
  const G4IonTable *theIonTable =
    G4ParticleTable::GetParticleTable()->GetIonTable();
  G4RadioactiveDecay *theRadioactiveDecay = new G4RadioactiveDecay();
  theRadioactiveDecay->SetVerboseLevel(0);
  theRadioactiveDecay->SetAnalogueMonteCarlo (true);
  theRadioactiveDecay->SetFBeta (true);
  for (G4int i=0; i<theIonTable->Entries(); i++)
  {
    G4String particleName = theIonTable->GetParticle(i)->GetParticleName();
    if (particleName == "GenericIon")
    {
      G4ProcessManager* pmanager =
        theIonTable->GetParticle(i)->GetProcessManager();
      pmanager->SetVerboseLevel(0);
      pmanager ->AddProcess(theRadioactiveDecay);
      pmanager ->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
      pmanager ->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
    }
  }
}

#include "G4AntiNeutronAnnihilationAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4PionMinusAbsorptionAtRest.hh"
#include "G4MuonMinusCaptureAtRest.hh"
// #include "G4NeutronCaptureAtRest.hh" // wrong physics, don't use! [GAHS]

#include "G4HadronElasticProcess.hh"   
#include "G4HadronInelasticProcess.hh" 
#include "G4HadronFissionProcess.hh"   
#include "G4HadronCaptureProcess.hh"         

#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPFission.hh"
#include "G4NeutronHPCapture.hh"

// high precision < 20 MeV
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPFissionData.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPInelasticData.hh"

// generic models for 0 to infinite energy (used for E > 20 MeV)
// (these may actually be "translations" of the GEANT3.21/GHEISHA models....)
#include "G4LElastic.hh"
#include "G4LFission.hh"
#include "G4LCapture.hh"

// Low energy (used for 20 MeV < E < 50 GeV)
#include "G4LENeutronInelastic.hh"  // 0 to 55 GeV

// High energy (used for > 50 GeV)
#include "G4HENeutronInelastic.hh"  // 45 GeV to 10 TeV


void AngraPhysicsList::ConstructHadronic()
{  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if ( particle == G4AntiNeutron::AntiNeutron() ) {
      pmanager->AddRestProcess(new G4AntiNeutronAnnihilationAtRest());
    }
    
    if ( particle == G4AntiProton::AntiProton() ) {
      pmanager->AddRestProcess(new G4AntiProtonAnnihilationAtRest());
    }
    
    if ( particle == G4KaonMinus::KaonMinus() ) {
      pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest());
    }
    
    if ( particle == G4PionMinus::PionMinus() ) {
      pmanager->AddRestProcess(new G4PionMinusAbsorptionAtRest());
    }

    if ( particle == G4MuonMinus::MuonMinus() ) {
      pmanager->AddRestProcess(new G4MuonMinusCaptureAtRest());
    }
    
    if ( particle == G4Neutron::Neutron() ) {
      
      // Elastic Processes
      G4HadronElasticProcess* theHadronElasticProcess = new   G4HadronElasticProcess();

      G4LElastic*         theNeutronLElastic  = new G4LElastic();
      G4NeutronHPElastic* theNeutronHPElastic = new G4NeutronHPElastic();
      
      theNeutronHPElastic->SetMaxEnergy( 20.*MeV );
      theNeutronLElastic->SetMinEnergy( 20.*MeV );
      
      theHadronElasticProcess->RegisterMe( theNeutronHPElastic );
      theHadronElasticProcess->RegisterMe( theNeutronLElastic );
      theHadronElasticProcess->AddDataSet(new G4NeutronHPElasticData());

      pmanager->AddDiscreteProcess( theHadronElasticProcess );

      // Inelastic Processes
      G4NeutronInelasticProcess* theNeutronInelasticProcess = new   G4NeutronInelasticProcess();

      G4LENeutronInelastic* theNeutronLENeutronInelastic = new   G4LENeutronInelastic();
      G4HENeutronInelastic* theNeutronHENeutronInelastic = new   G4HENeutronInelastic();
      G4NeutronHPInelastic* theNeutronHPInelastic        = new   G4NeutronHPInelastic();
      
      theNeutronHPInelastic->SetMaxEnergy( 20.*MeV );
      theNeutronLENeutronInelastic->SetMinEnergy( 20.*MeV );

      theNeutronInelasticProcess->RegisterMe( theNeutronHPInelastic );
      theNeutronInelasticProcess->RegisterMe( theNeutronLENeutronInelastic );
      theNeutronInelasticProcess->RegisterMe( theNeutronHENeutronInelastic );
      theNeutronInelasticProcess->AddDataSet(new G4NeutronHPInelasticData() );
    
      
      pmanager->AddDiscreteProcess( theNeutronInelasticProcess );
      
      // Fission Processes
      G4HadronFissionProcess* theHadronFissionProcess = new   G4HadronFissionProcess();
      G4LFission*         theNeutronLFission  = new G4LFission();
      G4NeutronHPFission* theNeutronHPFission = new G4NeutronHPFission();
      
      theNeutronHPFission->SetMaxEnergy( 20.*MeV );
      theNeutronLFission->SetMinEnergy( 20.*MeV );
      
      theHadronFissionProcess->RegisterMe( theNeutronHPFission );
      theHadronFissionProcess->RegisterMe( theNeutronLFission );
      theHadronFissionProcess->AddDataSet( new G4NeutronHPFissionData() );

      pmanager->AddDiscreteProcess( theHadronFissionProcess );

      // Capture Processes
      G4HadronCaptureProcess* theCaptureProcess = new G4HadronCaptureProcess();
      G4LCapture*         theNeutronLCapture  = new G4LCapture();
      G4NeutronHPCapture* theNeutronHPCapture = new G4NeutronHPCapture();

      theNeutronHPCapture->SetMaxEnergy( 20.*MeV );
      theNeutronLCapture->SetMinEnergy( 20.*MeV );
      
      theCaptureProcess->RegisterMe( theNeutronHPCapture );
      theCaptureProcess->RegisterMe( theNeutronLCapture );
      theCaptureProcess->AddDataSet( new G4NeutronHPCaptureData() );
      
      pmanager->AddDiscreteProcess( theCaptureProcess );            
    }
    
  }
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
      
void AngraPhysicsList::ConstructEM()
{

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
    // gamma
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());


      
    } else if (particleName == "e-") {
    //electron
      // Construct processes for electron
      pmanager->AddProcess(new G4eMultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4eIonisation(),-1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),-1,-1,3);

    } else if (particleName == "e+") {
    //positron
      // Construct processes for positron
      pmanager->AddProcess(new G4eMultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4eIonisation(),-1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung(),-1,-1,3);
      pmanager->AddProcess(new G4eplusAnnihilation(),0,-1,4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon
      // Construct processes for muon
      pmanager->AddProcess(new G4MuMultipleScattering(),-1,1,1);
      pmanager->AddProcess(new G4MuIonisation(),-1,2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);
      pmanager->AddProcess(new G4MuPairProduction(),-1,-1,4);
    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino")) {
     // all others charged particles except geantino
       pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
       pmanager->AddProcess(new G4hIonisation(),-1,2,2);
     }
    }
  }
}

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4UserSpecialCuts.hh"
 
void AngraPhysicsList::ConstructOp()
{
  G4Cerenkov*          theCerenkovProcess;
  G4Scintillation*     theScintillationProcess;
  G4OpAbsorption*      theAbsorptionProcess;
  G4OpRayleigh*        theRayleighScatteringProcess;
  G4OpBoundaryProcess* theBoundaryProcess;
  G4UserSpecialCuts*   theUserCuts;
  theUserCuts = new G4UserSpecialCuts();

  theCerenkovProcess      = new G4Cerenkov("Cerenkov");
  theScintillationProcess = new G4Scintillation("Scintillation");
  theAbsorptionProcess    = new G4OpAbsorption();
  theRayleighScatteringProcess = new G4OpRayleigh();
  theBoundaryProcess  = new G4OpBoundaryProcess();

  
  theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
  theCerenkovProcess->SetTrackSecondariesFirst(true);
  
  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetTrackSecondariesFirst(true);

  G4OpticalSurfaceModel themodel = unified;
  theBoundaryProcess->SetModel(themodel);

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theCerenkovProcess);
       pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
       }
    if (theScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theScintillationProcess);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
    } 
   //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types
   
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager -> AddDiscreteProcess(theAbsorptionProcess);
      pmanager -> AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager -> AddDiscreteProcess(theBoundaryProcess);
      //pmanager -> AddDiscreteProcess(theUserCuts);
    }
  }
}

//neutron spallation by muons
#include "G4MuNuclearInteraction.hh"
#include "G4StringChipsParticleLevelInterface.hh"
#include "G4PreCompoundModel.hh"
#include "G4BinaryCascade.hh"
#include "G4PhotoNuclearProcess.hh"
#include "G4GammaNuclearReaction.hh"
#include "G4ElectroNuclearReaction.hh"
#include "G4CascadeInterface.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"

#include "G4ElectronNuclearProcess.hh"
#include "G4PositronNuclearProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"

void AngraPhysicsList::ConstructNuclear()
{
    // ===============================================================
    // These processes are here mainly to take in account 
    // for the neutron spallation by cosmic muons.
    // One can turn off this effect by commenting the
    // "ConstructNuclear()" line at the ConstructProcess() method.
    //
    // Processes chosen from: 
    // H. Araújo and V. Kudryavtsev, 
    // http://www.unizar.es/ilias/wp2Back/Conferencias/IDM04/kudryavtsev_vitaly-neutrons.pdf
    // ===============================================================
    
    
    // ---------------------------------------------------------------
    // Quark-Gluon String Model: needed for internal nuclear reactions.
    // Aplies to: proton, neutron, pi+ and pi-    


    G4TheoFSGenerator* QGSC_model = new G4TheoFSGenerator();
    
    G4StringChipsParticleLevelInterface* theCascade = new G4StringChipsParticleLevelInterface();
    QGSC_model->SetTransport(theCascade);
    
    G4QGSMFragmentation* theFragmentation = new G4QGSMFragmentation(); // Instanciate a QGSM fragmentation process...   
    G4ExcitedStringDecay* theStringDecay = new G4ExcitedStringDecay(theFragmentation);// ...pass it to a StringDecay.
    
    G4QGSModel< G4QGSParticipants >* theStringModel = new G4QGSModel< G4QGSParticipants >;// Instanciate a StringModel...
    theStringModel->SetFragmentationModel(theStringDecay);// Pass StringDecay to the StringModel.    
    QGSC_model->SetHighEnergyGenerator(theStringModel);   // Pass StringModel to QGSP.
    
    QGSC_model->SetMinEnergy( 8.*GeV ); // This line is actually irrelevant.

    // ---------------------------------------------------------------
    // Binary Cascade Model
    // Aplies to: proton, neutron
    G4BinaryCascade* BC_model = new G4BinaryCascade();     
    BC_model -> SetMinEnergy( 70.*MeV );
    BC_model -> SetMaxEnergy( 8.0*GeV );

    // Aplies to: pi+, pi-    
    G4BinaryCascade* BC_Pion_model = new G4BinaryCascade(); 
    BC_Pion_model -> SetMaxEnergy( 1.3*GeV );

    // ---------------------------------------------------------------
    // Simple PreCompound model, used only to low energy processes.
    // Aplies to: proton, neutron. 
    G4ExcitationHandler* theHandler = new G4ExcitationHandler();
    G4PreCompoundModel*  PC_model = new G4PreCompoundModel(theHandler); // Low energy     (E < 70 MeV)
    PC_model -> SetMaxEnergy( 70.*MeV );
    
    // ---------------------------------------------------------------
    // Pion Plus Inelastic Process
    // Aplies to: pi+
    G4LEPionPlusInelastic* PPLE_model = new G4LEPionPlusInelastic();
    PPLE_model -> SetMinEnergy( 1.3*GeV );
    PPLE_model -> SetMaxEnergy( 8.0*GeV );
    
    // ---------------------------------------------------------------
    // Pion Minus Inelastic Process
    // Aplies to: pi-
    G4LEPionMinusInelastic* PMLE_model = new G4LEPionMinusInelastic();
    PMLE_model -> SetMinEnergy( 1.3*GeV );
    PMLE_model -> SetMaxEnergy( 8.0*GeV );

    // ---------------------------------------------------------------
    // Electro Nuclear Reactions
    // Aplies to: e+ and e-
    G4ElectroNuclearReaction* electroNuclear = new G4ElectroNuclearReaction();

    // ---------------------------------------------------------------
    // Gamma Nuclear Reaction, uses CHIPS
    // Aplies to: gamma
    G4GammaNuclearReaction* gammaNuclear = new G4GammaNuclearReaction();
    //gammaNuclear -> SetMaxEnergy( 3.0*MeV );
    
    // ---------------------------------------------------------------
    // Bertini Model
    // Aplies to: gamma
    G4CascadeInterface* bertiniModel;
    bertiniModel = new G4CascadeInterface();
    gammaNuclear -> SetMinEnergy( 3.0*MeV );


    theParticleIterator->reset();
    while ((*theParticleIterator)()){

	  G4ParticleDefinition* particle = theParticleIterator->value();
	  G4ProcessManager* pmanager = particle->GetProcessManager();
	  G4String particleName = particle->GetParticleName();
      
	  if (particleName == "gamma") {
	     G4PhotoNuclearProcess*  photoNuclearProcess = new G4PhotoNuclearProcess();
             photoNuclearProcess->RegisterMe(gammaNuclear);
//	     photoNuclearProcess->RegisterMe(bertiniModel);
             pmanager->AddDiscreteProcess(photoNuclearProcess);    
	  } 
	  
	  if (particleName == "e-") {
	    G4ElectronNuclearProcess* theElectronNuclearProcess = new G4ElectronNuclearProcess();
	    theElectronNuclearProcess->RegisterMe(electroNuclear);
	    pmanager->AddProcess(theElectronNuclearProcess, -1, -1, 5);

	  } 
	  
	  if (particleName == "e+") {
	    G4PositronNuclearProcess* thePositronNuclearProcess = new G4PositronNuclearProcess();
	    thePositronNuclearProcess->RegisterMe(electroNuclear);
	    pmanager->AddProcess(thePositronNuclearProcess, -1, -1, 5);
	  }

          if (particleName == "mu+" || particleName == "mu-" ) 
	  {
             pmanager->AddDiscreteProcess(new G4MuNuclearInteraction());    
          } 
          
	    
          if ( particleName == "proton" ) {
            G4ProtonInelasticProcess* theProtonInelasticProcess = new G4ProtonInelasticProcess();
            theProtonInelasticProcess-> RegisterMe(PC_model);   // E < 70 MeV
            theProtonInelasticProcess-> RegisterMe(BC_model);   // 70 MeV < E < 8  GeV
            theProtonInelasticProcess-> RegisterMe(QGSC_model); // E > 8 GeV
            pmanager->AddDiscreteProcess(theProtonInelasticProcess);
	  }    
	  
          if ( particleName == "neutron" ) {
            G4NeutronInelasticProcess* theNeutronInelasticProcess = new   G4NeutronInelasticProcess();
            theNeutronInelasticProcess-> RegisterMe(PC_model);   // E < 70 MeV
	    theNeutronInelasticProcess-> RegisterMe(BC_model);   // 70 MeV < E < 8  GeV  
            theNeutronInelasticProcess-> RegisterMe(QGSC_model); // E > 8 GeV
            pmanager->AddDiscreteProcess(theNeutronInelasticProcess);
	     
	  }

	  if ( particleName == "pi-" )
	  {
             G4PionMinusInelasticProcess* thePionMinusInelasticProcess = new G4PionMinusInelasticProcess();
	     thePionMinusInelasticProcess-> RegisterMe(BC_Pion_model); // E < 1.3 GeV
             thePionMinusInelasticProcess-> RegisterMe(PMLE_model);    // 1.3 GeV < E < 8  GeV  
             thePionMinusInelasticProcess-> RegisterMe(QGSC_model);    // E > 8 GeV   
             pmanager->AddDiscreteProcess( thePionMinusInelasticProcess );    
	  }
	  if ( particleName == "pi+" )
	  {
	     G4PionPlusInelasticProcess* thePionPlusInelasticProcess = new G4PionPlusInelasticProcess();
	     thePionPlusInelasticProcess-> RegisterMe(BC_Pion_model); // E < 1.3 GeV
	     thePionPlusInelasticProcess-> RegisterMe(PMLE_model);    // 1.3 GeV < E < 8  GeV  
             thePionPlusInelasticProcess-> RegisterMe(QGSC_model);    // E > 8 GeV   
	     pmanager->AddDiscreteProcess( thePionPlusInelasticProcess );
	  }
	  
/*          if ( particleName == "K+" )
	  {
             G4KaonMinusInelasticProcess* theKaonMinusInelasticProcess = new G4KaonMinusInelasticProcess();
             theKaonMinusInelasticProcess-> RegisterMe(QGSP_model);      
             pmanager->AddDiscreteProcess( theKaonMinusInelasticProcess );
	  }

          if ( particleName == "K-" )
	  {
	     G4KaonPlusInelasticProcess* theKaonPlusInelasticProcess = new G4KaonPlusInelasticProcess();
	     theKaonPlusInelasticProcess-> RegisterMe(QGSP_model);
             pmanager->AddDiscreteProcess( theKaonPlusInelasticProcess );
	  }
*/  
     }           
}



void AngraPhysicsList::SetCuts()
{

  SetCutsWithDefault();

  if (verboseLevel>0)   DumpCutValuesTable();
    
}

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

void AngraPhysicsList::AddLimits()
{
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (particle->GetParticleName() == "opticalphoton")
      { 
         std::cout << " limiting " <<  particle->GetParticleName() << std::endl;
         pmanager ->AddDiscreteProcess(stepLimiter);
         pmanager ->AddDiscreteProcess(userCuts);
      }
  }
  
}


