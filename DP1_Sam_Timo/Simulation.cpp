#include "Simulation.h"
#include "States\InitState.h"

Simulation::Simulation()
{
	this->pState = std::make_shared<InitState>(this);
}

Simulation::~Simulation()
{
	this->Cleanup();
}

// laat file reader de file lezen.
int Simulation::Load()
{
	return pReader->Read();
}

void Simulation::FailedLoad()
{
	this->pOutput->Print("Failed to load file.");
}

void Simulation::FailedCircuit()
{
	this->pOutput->Print("Failed to create circuit.");
}

// maak een circuit
int Simulation::CreateCircuit()
{
	this->pCircuit = std::make_shared<Circuit>();
	this->pCircuit->SetOutput(this->pOutput);
	this->pCircuit->SetVisitor(this->pVisitor);
	// kijk of alles klopt
	if (!pCircuit->CreateNodes(pReader->GetNodes()) || !pCircuit->CreateInputs(pReader->GetInputs()) || !pCircuit->CreateProbes(pReader->GetProbes()) || !pCircuit->CreateEdges(pReader->GetEdges()))
		return 0;

	return 1;
}

// start de circuit.
void Simulation::Start(std::string Filename)
{

	this->_fileName = Filename;
	// loop omdat we mogelijk opnieuw willen	
	while (this->_running)
	{
		this->pState->Update();
	}

	/*while (true)
	{
		// init circuit en alles
		this->Init();

		

		// laad data uit de file
		if (Load())
		{
			// maak circuit
			if (CreateCircuit()) {
				pOutput->Print("Initializing the circuit diagram!");
				pOutput->Print("Started the circuit!");
				pCircuit->Start();
				if(pCircuit->IsSuccesful() && ShowDiagram()) 
					pDiagramGenerator->Generate(*pCircuit);
			}
			else
			{
				pOutput->Print("Could not create the circuit!");
				break;
			}
		}
		else
		{
			pOutput->Print("Failed loading the file!");
			break;
		}

		// opnieuw?
		rerun = RunAgain();
		if (!rerun)
			break;

		// ruim alles op
		this->Cleanup();
	}*/
}

// wordt gebruikt voor unit tests
// alles zonder checks uitvoeren om de errors te krijgen
std::vector<Probe *> Simulation::StartTest(std::string circuit)
{
	std::vector<Probe *> probes;
	this->Init();
	this->pReader = Filereader::CreateStringreader(circuit);

	auto isLoaded = this->Load();
	if(!isLoaded)
		return probes;
	
	this->pCircuit = std::make_shared<Circuit>();
	this->pCircuit->SetInputs(1, 0);
	this->pCircuit->SetOutput(this->pOutput);
	this->pCircuit->SetVisitor(this->pVisitor);
	if (!pCircuit->CreateNodes(pReader->GetNodes()) || !pCircuit->CreateInputs(pReader->GetInputs()) || !pCircuit->CreateProbes(pReader->GetProbes()) || !pCircuit->CreateEdges(pReader->GetEdges()))
		return probes;

	probes = pCircuit->Start();

	return probes;
}

bool Simulation::ShowDiagram()
{
	pOutput->Print("Generate diagram? y/n");
	return pOutput->RunAgain() == "y";
}

// vraag of we nog een keer willen
int Simulation::RunAgain()
{
	pOutput->Print("Run the circuit again? y/n");
	return pOutput->RunAgain() == "y";
}

void Simulation::Run()
{
	pOutput->Print("Started the circuit!");
	pCircuit->Start();
	if (pCircuit->IsSuccesful() && ShowDiagram())
		pDiagramGenerator->Generate(*pCircuit);
}

// init alles
void Simulation::Init()
{
	pOutput = std::make_shared<Output>();
	pDiagramGenerator = std::make_shared<DiagramGeneration::CppGenerator>();
	pVisitor = std::make_shared<Visitor>();
	pVisitor->SetOutput(this->pOutput);
	pOutput->Print("Initializing the circuit diagram!");
	// zet de file
	this->pReader = Filereader::CreateFilereader(_fileName);
}

void Simulation::Cleanup()
{
	if (pReader != nullptr)
	{
		delete pReader;
		pReader = nullptr;
	}
}

void Simulation::SetRunning(bool running)
{
	this->_running = running;
}

void Simulation::SetState(std::shared_ptr<State> pState)
{
	this->pState = pState;
}
