var Cathedrale_01 = Animation.extend({

    setup: function (s) {

        println("-- Cathedrale_01.js -- ");
        this._super();

        

    },

    update: function (s) {
        
        setOrientation(s, 0, 45, 45, -5);
        setOrientation(s, 16, 45, -45, -5);
        
        
        setOrientation(s, 20, 45, -45, -5);
        setOrientation(s, 4, 45, 45, -5);
        
        setOrientation(s, 24, 45, -45, -5);
        setOrientation(s, 8, 45, 45, -5);
        
        setOrientation(s, 28, 45, -45, -5);
        setOrientation(s, 12, 45, 45, -5);
    
        
        //forceLight(s, i, 0);


    }



});


new Cathedrale_01();




