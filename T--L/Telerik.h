Assembly<Telisk>

class Telerik
{
	void appendTasnal(Tasnal*);
	void registerTasnal(Tasnal*);
	void setTeshnal(Tasnal&);
	//Intiful& Type settin',External
	void (*Delegate)(Tasnal*) = registerTasnal;
	//void(*Delegate)(Tasnal&) { &appendTasnal};
}
