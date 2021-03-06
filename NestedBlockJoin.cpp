#include "NestedBlockJoin.h"

#include "BufferManager.h"

NestedBlockJoin::NestedBlockJoin(IRelationalOperator * r1, IRelationalOperator * r2,
		JoinClause * clause) : JoinOperator(r1,r2,clause) {
	m_buffer = BufferManager::getInstance()->allocate();
	m_data = new byte[m_schema.rsize()];
	m_next[LEFT] = 0;
	m_next[RIGHT] = 0;
	m_childTuple[LEFT].m_data = new byte[r1->schema()->rsize()];
	m_childTuple[RIGHT].m_data = new byte[r2->schema()->rsize()];
	
	for(int i = 0; i < 2; i++) {
		// Allocate
		m_childBuffer[i] = BufferManager::getInstance()->allocate();
	}
}

NestedBlockJoin::~NestedBlockJoin() {
	if (m_buffer != NULL) {
		BufferManager::getInstance()->deallocate(m_buffer);
    }

	for(int i = 0; i < 2; i++) {
		BufferManager::getInstance()->deallocate(m_buffer);
		delete[] m_childTuple[i].m_data;
	}
	
	delete[] m_data;
}

void NestedBlockJoin::next(MemoryBlock & block) {
	block.copy(*m_buffer);
}

bool NestedBlockJoin::nextTuple(int branch, Tuple * tuple) {
	bool hasNext = false;
	if(m_next[branch] < m_childBuffer[branch]->getSize()) {
		// retrieve tuple from memory buffer.
		hasNext = true;
	}
	else {
		if(m_child[branch]->moveNext()) {
			m_child[branch]->next(*m_childBuffer[branch]);
			m_next[branch] = 0;
			hasNext = true;
		}
	}
	
	if(hasNext) {
		m_childBuffer[branch]->get(tuple->m_data, 
			    m_next[branch] * m_child[branch]->schema()->rsize(), 
			    m_child[branch]->schema()->rsize());
	}
	return hasNext;
}

bool NestedBlockJoin::moveNext() {

	int nrecords = 0;
	int offset = 0;
	int available = m_buffer->capacity();
	int rsize = m_schema.rsize();
  	
	m_buffer->clear();
	
	// Reset inner loop & increment outer loop if we reached the end
	while(rsize < available && nextTuple(LEFT, &m_childTuple[LEFT])) {
		while(rsize < available && nextTuple(RIGHT, &m_childTuple[RIGHT])) {
			
			if(m_clause == NULL || m_clause->evaluate(m_childTuple[LEFT], m_childTuple[RIGHT])) {
				// Merge new tuple into temporary m_data location
				memcpy(m_data, m_childTuple[LEFT].m_data, m_child[LEFT]->schema()->rsize());
				memcpy(m_data + m_child[LEFT]->schema()->rsize(), m_childTuple[RIGHT].m_data, m_child[RIGHT]->schema()->rsize());
			
				// Copy tuple to memory buffer
				m_buffer->put(m_data, offset, rsize);
						
				// book-keeping.
				offset += rsize;
				available -= rsize;
				nrecords++;
				
			}
			
			m_next[RIGHT]++;
			
			// Return now so we dont increment if we are exiting due to no more room in buffer
			if(rsize >= available) {
				m_buffer->setSize(nrecords);
				return true;
			}
			
		}
		
		m_child[RIGHT]->reset();
		m_next[LEFT]++;

	}

	m_buffer->setSize(nrecords);
	return  nrecords > 0;
}

void NestedBlockJoin::reset() {
	// Reset both relations to reset join
	m_child[LEFT]->reset();
	m_child[RIGHT]->reset();
}
