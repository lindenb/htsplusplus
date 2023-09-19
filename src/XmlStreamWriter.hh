class XmlStreamWriter {
	protected:
		std::ostream& out;
	private:
		class State {
			public:
				std::string element_name;
				XmlStreamWriter* owner;
				State* parent;
				State* child;
				bool empty_element;
				State(XmlStreamWriter* owner,State* parent):owner(owner),parent(parent),child(NULL),empty_element(false); {
					}
				~State() {
					if(child!=NULL) delete child;
					}
			};
		State* current;
		State* push() {
			State* c = new State(this,current);
			current =c;
			return c;
			}
		State* pop() {
			current = current->parent;
			delete parent->child;
			current->child=NULL;
			return current;
			}
	public:
		XmlStreamWriter(std::ostream& out):out(out) {
			current = new State(this,NULL);
			}

		void writeStateDocument(const char* version,const char* encoding) {
			}

		void writeStartDocument(const char* encoding) {
			writeStartDocument("1.0",encoding);
			}
		void writeStartDocument() {
			writeStartDocument("UTF-8");
			}

		void writeEndDocument() {
			out.flush();
			}
		void writeStartElement(const char* s) {
			push();
			current->element_name.assign(s);
			out.write("<");
			out << s;
			}
		void writeEndElement() {
			out << "</" << current->element_name << ">";
			pop();
			}
		void writeAttribute(const char* name, const char* value) {
			out << " " << name << "=\"" ;
			StringUtils::escapeXml(value,out);
			out << "\"";
			}
		void writeCharacters(const char* s) {
			if(curr->!closed) {
				if(curr->is_empty_element) {
					out << "/";
					pop();
					}
				out << ">";
				}
			StringUtils::escapeXml(s,out);
			}
		
		virtual ~XmlStreamWriter() {
			}

	};
