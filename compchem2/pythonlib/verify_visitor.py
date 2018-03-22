
import visitor

class verify_visitor(visitor.visitor):
    """Perform the verification actions on a database.
    
    for each project
       open project // will auto-fix link
    
    for each memo
       open memo // will auto-fix link
    
    Use ID/IDREF tables to verify that the following reference attributes have a target object:
     * compound-ref -> compound/formula
     * experiment -> worker
     * memo -> mechanism/filter, compound/formula
     * project -> worker
     * settings -> (parent)/filter"""
    def operator=():
        """no assign"""
        pass
        
    def report_cross_references():
        """Print a report on the status of cross-references."""
        pass
        
    def verify_visitor():
        pass
        
    def ~verify_visitor():
        pass
        
    def verify_visitor():
        """no copy"""
        pass
        
    def __init__(self):
        super(verify_visitor, self).__init__()
        pass
    
