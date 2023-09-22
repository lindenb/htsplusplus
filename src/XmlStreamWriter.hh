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
    
        void closeCurr() {
              if(curr->!closed) {
			    if(curr->is_empty_element) {
				    out << "/";
				    pop();
				    }
			    out << ">";
                curr->!closed = true;
			    }
            }		

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
        void prepareText() {
             closeCurr();
             if(current->parent==NULL) THROW_ERROR("boum");            
             }
	public:
		XmlStreamWriter(std::ostream& out):out(out) {
			current = new State(this,NULL);
			}

		void writeStartDocument(const char* version,const char* encoding) {
            if(current->parent!=NULL) THROW_ERROR("boum");
            out << "<?xml version=\"" << version << "\" encoding=\"" << endoding << "?>";
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
            if(curr->paren!=NULL)  curr->parent->contains_node = true;
			}
		void writeEndElement() {
			out << "</" << current->element_name << ">";
			pop();
			}
		void writeAttribute(const char* name, const char* value) {
            if(curr->closed) THROW_ERROR(""); 
            if(curr->contains_node) THROW_ERROR(""); 
			out << " " << name << "=\"" ;
			StringUtils::escapeXml(value,out);
			out << "\"";
			}
		void writeCharacters(const char* s) {
			prepareText();
			StringUtils::escapeXml(out,s);
            curr->contains_node = true;
			}
		void writeComment(const char* s) {
            prepareText();
            out << "<!-- " << s << " -->";
             curr->contains_node = true;
            }
        void writeCDATA(const char* s) {
            out << "<CDATA[[" << s << "]]>";
             curr->contains_node = true;
            }
        void close() {
            while(curr->parent!=NULL) {
                 writeEndElement();
                 }
            }
        void flush() {
             }
		virtual ~XmlStreamWriter() {
            close();
            flush();
			}

	};
